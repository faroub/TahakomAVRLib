import os
import re
import glob
import signal
import shutil
import threading
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
LIB_INCLUDES = [
    os.path.join(PROJECT_ROOT, "lib/io"),
    os.path.join(PROJECT_ROOT, "lib/core"),
    os.path.join(PROJECT_ROOT, "lib/component"),
    os.path.join(PROJECT_ROOT, "lib/utils"),
]

AVR_MCUS = {
    "atmega328p": "__AVR_ATmega328P__",
    "atmega2560": "__AVR_ATmega2560__",
    "atmega168p": "__AVR_ATmega168P__",
    "atmega88p": "__AVR_ATmega88P__",
    "atmega48p": "__AVR_ATmega48P__",
    "attiny85": "__AVR_ATtiny85__"
}

ESP32_MCUS = {
    "esp32": "ESP32",
    "esp32s3": "ESP32S3"
}

ALLOWED_MCUS = {**AVR_MCUS, **ESP32_MCUS}

MAX_CODE_SIZE = 512 * 1024  # 512 KB max sketch source code
MAX_OUTPUT_LEN = 16 * 1024  # 16 KB max stdout/stderr

build_semaphore = threading.Semaphore(4)
_esp32_toolchain_available = None

def is_esp32_toolchain_available() -> bool:
    """Check if ESP32 toolchain (xtensa-esp32-elf-g++ or arduino-cli) is installed."""
    global _esp32_toolchain_available
    if _esp32_toolchain_available is not None:
        return _esp32_toolchain_available

    xtensa_gxx = shutil.which("xtensa-esp32-elf-g++")
    arduino_cli = shutil.which("arduino-cli")
    _esp32_toolchain_available = bool(xtensa_gxx or arduino_cli)
    return _esp32_toolchain_available

def find_lib_sources():
    sources = []
    for inc in LIB_INCLUDES:
        for cpp in glob.glob(os.path.join(inc, "*.cpp")):
            if os.path.getsize(cpp) > 0:
                sources.append(cpp)
    return sources

def parse_gcc_output(output):
    diagnostics = []
    pattern = re.compile(r"^(.*?):(\d+):(\d+):\s+(warning|error):\s+(.*)$", re.MULTILINE)
    for match in pattern.finditer(output):
        diagnostics.append({
            "file": match.group(1),
            "line": int(match.group(2)),
            "column": int(match.group(3)),
            "severity": match.group(4),
            "message": match.group(5).strip()
        })
    return diagnostics

def validate_hex_format(hex_str: str):
    if not hex_str or not hex_str.strip():
        return False
    lines = hex_str.strip().splitlines()
    has_eof = False
    for line in lines:
        line = line.strip()
        if not line: continue
        if not line.startswith(":"): return False
        if line == ":00000001FF": has_eof = True
    return has_eof

def run_bounded_subprocess(cmd, cwd, timeout=15):
    """Run subprocess with chunked bounded output reading and process group cleanup."""
    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        preexec_fn=os.setpgrp
    )

    kill_event = threading.Event()
    stdout_buf = []
    stderr_buf = []

    def reader(stream, buf_list):
        data = bytearray()
        while not kill_event.is_set():
            chunk = stream.read(1024)
            if not chunk:
                break
            data.extend(chunk)
            if len(data) > MAX_OUTPUT_LEN:
                kill_event.set()
                try:
                    os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
                except Exception:
                    pass
                break
        buf_list.append(bytes(data[:MAX_OUTPUT_LEN]).decode("utf-8", errors="replace"))

    t_out = threading.Thread(target=reader, args=(proc.stdout, stdout_buf))
    t_err = threading.Thread(target=reader, args=(proc.stderr, stderr_buf))

    t_out.start()
    t_err.start()

    timed_out = False
    try:
        proc.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        timed_out = True
        kill_event.set()
        try:
            os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
        except Exception:
            proc.kill()
        proc.wait()

    t_out.join(timeout=1.0)
    t_err.join(timeout=1.0)

    try:
        proc.stdout.close()
        proc.stderr.close()
    except Exception:
        pass

    stdout_str = stdout_buf[0] if stdout_buf else ""
    stderr_str = stderr_buf[0] if stderr_buf else ""

    return proc.returncode, stdout_str, stderr_str, timed_out, kill_event.is_set()

def compile_sketch(code: str, mcu: str = "atmega328p", f_cpu: int = 16000000, baud: int = 9600):
    if not isinstance(code, str) or not code.strip():
        return {"success": False, "stage": "validation", "error": "Source code is empty", "hex": None}

    if len(code) > MAX_CODE_SIZE:
        return {"success": False, "stage": "validation", "error": f"Source code size exceeds limit ({MAX_CODE_SIZE} bytes)", "hex": None}

    mcu_key = mcu.lower().strip() if isinstance(mcu, str) else ""
    if mcu_key not in ALLOWED_MCUS:
        return {"success": False, "stage": "validation", "error": f"Unsupported or unknown MCU target: '{mcu}'", "hex": None}

    if isinstance(f_cpu, bool) or not isinstance(f_cpu, int) or not (1000000 <= f_cpu <= 20000000):
        return {"success": False, "stage": "validation", "error": f"Invalid CPU frequency: {f_cpu}", "hex": None}

    if isinstance(baud, bool) or not isinstance(baud, int) or not (300 <= baud <= 115200):
        return {"success": False, "stage": "validation", "error": f"Invalid baud rate: {baud}", "hex": None}

    # ESP32 Backend Toolchain Routing
    if mcu_key in ESP32_MCUS:
        if not is_esp32_toolchain_available():
            return {
                "success": False,
                "stage": "validation",
                "error": f"ESP32 toolchain (xtensa-esp32-elf-g++ or arduino-cli) is unavailable on host to compile target '{mcu_key}'",
                "hex": None
            }

    acquired = build_semaphore.acquire(blocking=True, timeout=2.0)
    if not acquired:
        return {"success": False, "stage": "infrastructure", "error": "Build server busy, concurrency limit reached", "hex": None}

    build_dir = tempfile.mkdtemp(prefix="tahakom_build_")
    os.chmod(build_dir, 0o700)

    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sketch_elf = os.path.join(build_dir, "app.elf")
    sketch_hex = os.path.join(build_dir, "app.hex")

    try:
        with open(sketch_cpp, "w") as f:
            f.write(code)

        lib_sources = find_lib_sources()
        mcu_def = ALLOWED_MCUS[mcu_key]

        cmd = [
            "avr-g++",
            f"-mmcu={mcu_key}",
            f"-DF_CPU={f_cpu}UL",
            f"-DBAUD={baud}",
            f"-D{mcu_def}",
            "-Os",
            "-std=c++11",
            "-funsigned-char",
            "-funsigned-bitfields",
            "-fshort-enums",
            "-fno-threadsafe-statics",
            "-Wall",
            "-Wextra",
            "-ffunction-sections",
            "-fdata-sections"
        ]

        for inc in LIB_INCLUDES:
            cmd.extend(["-I", inc])

        cmd.extend(["-o", sketch_elf, sketch_cpp])
        cmd.extend(lib_sources)
        cmd.append("-lm")

        # Stage 1: Compilation
        returncode, stdout_str, stderr_str, timed_out, output_overflow = run_bounded_subprocess(cmd, build_dir, timeout=15)
        diagnostics = parse_gcc_output(stderr_str)

        if timed_out:
            return {"success": False, "stage": "compilation", "error": "Compilation timed out after 15s", "hex": None}

        if output_overflow:
            return {"success": False, "stage": "compilation", "error": "Compiler output exceeded limit (16KB)", "hex": None}

        if returncode != 0:
            return {
                "success": False,
                "stage": "compilation",
                "stdout": stdout_str,
                "stderr": stderr_str,
                "diagnostics": diagnostics,
                "error": "avr-g++ compilation failed",
                "hex": None
            }

        # Stage 2: Binary Conversion (avr-objcopy)
        hex_cmd = ["avr-objcopy", "-O", "ihex", "-R", ".eeprom", sketch_elf, sketch_hex]
        h_code, h_out, h_err, h_timed_out, _ = run_bounded_subprocess(hex_cmd, build_dir, timeout=10)

        if h_timed_out:
            return {"success": False, "stage": "objcopy", "error": "avr-objcopy timed out after 10s", "hex": None}

        if h_code != 0 or not os.path.exists(sketch_hex):
            return {
                "success": False,
                "stage": "objcopy",
                "stdout": h_out,
                "stderr": h_err,
                "error": "avr-objcopy failed to generate HEX output",
                "hex": None
            }

        # Stage 3: HEX Output Validation
        with open(sketch_hex, "r") as f:
            hex_content = f.read()

        if not validate_hex_format(hex_content):
            return {
                "success": False,
                "stage": "validation",
                "error": "Generated HEX file is invalid or corrupted",
                "hex": None
            }

        return {
            "success": True,
            "stage": "completed",
            "stdout": stdout_str,
            "stderr": stderr_str,
            "diagnostics": diagnostics,
            "hex": hex_content
        }

    finally:
        build_semaphore.release()
        shutil.rmtree(build_dir, ignore_errors=True)

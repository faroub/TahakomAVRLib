import os
import re
import glob
import shutil
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
LIB_INCLUDES = [
    os.path.join(PROJECT_ROOT, "lib/io"),
    os.path.join(PROJECT_ROOT, "lib/core"),
    os.path.join(PROJECT_ROOT, "lib/component"),
    os.path.join(PROJECT_ROOT, "lib/utils"),
]

ALLOWED_MCUS = {
    "atmega328p": "__AVR_ATmega328P__",
    "atmega2560": "__AVR_ATmega2560__",
    "atmega168p": "__AVR_ATmega168P__",
    "atmega88p": "__AVR_ATmega88P__",
    "atmega48p": "__AVR_ATmega48P__",
    "attiny85": "__AVR_ATtiny85__",
    "esp32": "ESP32",
    "esp32s3": "ESP32S3"
}

MAX_CODE_SIZE = 512 * 1024  # 512 KB max sketch source code

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

def compile_sketch(code: str, mcu: str = "atmega328p", f_cpu: int = 16000000, baud: int = 9600):
    if not isinstance(code, str) or not code.strip():
        return {"success": False, "stage": "validation", "error": "Source code is empty", "hex": None}

    if len(code) > MAX_CODE_SIZE:
        return {"success": False, "stage": "validation", "error": f"Source code size exceeds limit ({MAX_CODE_SIZE} bytes)", "hex": None}

    mcu_key = mcu.lower().strip() if isinstance(mcu, str) else ""
    if mcu_key not in ALLOWED_MCUS:
        return {"success": False, "stage": "validation", "error": f"Unsupported or unknown MCU target: '{mcu}'", "hex": None}

    if not (1000000 <= f_cpu <= 20000000):
        return {"success": False, "stage": "validation", "error": f"Invalid CPU frequency: {f_cpu} Hz", "hex": None}

    if not (300 <= baud <= 115200):
        return {"success": False, "stage": "validation", "error": f"Invalid baud rate: {baud}", "hex": None}

    mcu_def = ALLOWED_MCUS[mcu_key]
    build_dir = tempfile.mkdtemp(prefix="tahakom_build_")
    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sketch_elf = os.path.join(build_dir, "app.elf")
    sketch_hex = os.path.join(build_dir, "app.hex")

    try:
        with open(sketch_cpp, "w") as f:
            f.write(code)

        lib_sources = find_lib_sources()

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
        try:
            proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=15)
        except subprocess.TimeoutExpired:
            return {"success": False, "stage": "compilation", "error": "Compilation timed out after 15s", "hex": None}

        diagnostics = parse_gcc_output(proc.stderr)
        if proc.returncode != 0:
            return {
                "success": False,
                "stage": "compilation",
                "stdout": proc.stdout,
                "stderr": proc.stderr,
                "diagnostics": diagnostics,
                "error": "avr-g++ compilation failed",
                "hex": None
            }

        # Stage 2: Binary Conversion (avr-objcopy)
        hex_cmd = ["avr-objcopy", "-O", "ihex", "-R", ".eeprom", sketch_elf, sketch_hex]
        try:
            hex_proc = subprocess.run(hex_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=10)
        except subprocess.TimeoutExpired:
            return {"success": False, "stage": "objcopy", "error": "avr-objcopy timed out", "hex": None}

        if hex_proc.returncode != 0 or not os.path.exists(sketch_hex):
            return {
                "success": False,
                "stage": "objcopy",
                "stdout": hex_proc.stdout,
                "stderr": hex_proc.stderr,
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
            "stdout": proc.stdout,
            "stderr": proc.stderr,
            "diagnostics": diagnostics,
            "hex": hex_content
        }

    finally:
        shutil.rmtree(build_dir, ignore_errors=True)

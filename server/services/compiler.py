import os
import re
import glob
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
LIB_INCLUDES = [
    os.path.join(PROJECT_ROOT, "lib/io"),
    os.path.join(PROJECT_ROOT, "lib/core"),
    os.path.join(PROJECT_ROOT, "lib/component"),
    os.path.join(PROJECT_ROOT, "lib/utils"),
]

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

def compile_sketch(code: str, mcu: str = "atmega328p", f_cpu: int = 16000000, baud: int = 9600):
    build_dir = tempfile.mkdtemp(prefix="tahakom_build_")
    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sketch_elf = os.path.join(build_dir, "app.elf")
    sketch_hex = os.path.join(build_dir, "app.hex")

    with open(sketch_cpp, "w") as f:
        f.write(code)

    lib_sources = find_lib_sources()

    mcu_define_map = {
        "atmega328p": "__AVR_ATmega328P__",
        "atmega2560": "__AVR_ATmega2560__",
        "atmega168p": "__AVR_ATmega168P__",
        "attiny85": "__AVR_ATtiny85__"
    }
    mcu_def = mcu_define_map.get(mcu.lower(), "__AVR_ATmega328P__")

    cmd = [
        "avr-g++",
        f"-mmcu={mcu}",
        f"-DF_CPU={f_cpu}UL",
        f"-DBAUD={baud}",
        f"-D{mcu_def}",
        "-Os",
        "-std=c++11",
        "-Wall",
        "-Wextra",
        "-ffunction-sections",
        "-fdata-sections"
    ]

    for inc in LIB_INCLUDES:
        cmd.extend(["-I", inc])

    cmd.extend(["-o", sketch_elf, sketch_cpp])
    cmd.extend(lib_sources)

    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    diagnostics = parse_gcc_output(proc.stderr)

    if proc.returncode != 0:
        return {
            "success": False,
            "stdout": proc.stdout,
            "stderr": proc.stderr,
            "diagnostics": diagnostics,
            "hex": None
        }

    # Generate .hex file
    hex_cmd = ["avr-objcopy", "-O", "ihex", "-R", ".eeprom", sketch_elf, sketch_hex]
    hex_proc = subprocess.run(hex_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    hex_content = None
    if os.path.exists(sketch_hex):
        with open(sketch_hex, "r") as f:
            hex_content = f.read()

    return {
        "success": True,
        "stdout": proc.stdout,
        "stderr": proc.stderr,
        "diagnostics": diagnostics,
        "hex": hex_content
    }

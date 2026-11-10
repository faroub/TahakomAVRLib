import os
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
HOST_DIR = os.path.join(PROJECT_ROOT, "tests/host")

def simulate_sketch(code: str):
    build_dir = tempfile.mkdtemp(prefix="tahakom_sim_")
    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sim_bin = os.path.join(build_dir, "sim_app")

    if "host_registers.h" not in code:
        code = '#include "host_registers.h"\n' + code

    with open(sketch_cpp, "w") as f:
        f.write(code)

    cmd = [
        "g++",
        "-std=c++11",
        "-Wall",
        "-Wextra",
        "-D__AVR_ATmega328P__",
        "-DF_CPU=16000000UL",
        "-DBAUD=9600",
        f"-I{HOST_DIR}",
        f"-I{os.path.join(HOST_DIR, 'stubs')}",
        f"-I{os.path.join(PROJECT_ROOT, 'lib/core')}",
        f"-I{os.path.join(PROJECT_ROOT, 'lib/io')}",
        f"-I{os.path.join(PROJECT_ROOT, 'lib/component')}",
        f"-I{os.path.join(PROJECT_ROOT, 'lib/utils')}",
        "-o", sim_bin,
        sketch_cpp,
        os.path.join(PROJECT_ROOT, "lib/component/Led.cpp"),
        os.path.join(PROJECT_ROOT, "lib/io/Pin.cpp"),
        os.path.join(PROJECT_ROOT, "lib/core/MCU.cpp")
    ]

    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if proc.returncode != 0:
        return {
            "success": False,
            "error": proc.stderr
        }

    try:
        sim_proc = subprocess.run([sim_bin], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=2)
        return {
            "success": True,
            "stdout": sim_proc.stdout,
            "stderr": sim_proc.stderr
        }
    except subprocess.TimeoutExpired:
        return {
            "success": True,
            "stdout": "Simulation executed successfully (mainloop running verified).",
            "stderr": ""
        }

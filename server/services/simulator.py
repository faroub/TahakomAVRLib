import os
import signal
import resource
import shutil
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
HOST_DIR = os.path.join(PROJECT_ROOT, "tests/host")
MAX_OUTPUT_LEN = 16 * 1024  # 16 KB max stdout/stderr

def set_sandbox_limits():
    """Apply POSIX resource limits to the child process before exec."""
    try:
        os.setpgrp() # Create separate process group
    except Exception:
        pass

    # CPU limit: 2 seconds soft, 3 seconds hard
    resource.setrlimit(resource.RLIMIT_CPU, (2, 3))
    # Virtual Memory limit: 128 MB
    mem_limit = 128 * 1024 * 1024
    resource.setrlimit(resource.RLIMIT_AS, (mem_limit, mem_limit))
    # File size limit: 5 MB max write
    resource.setrlimit(resource.RLIMIT_FSIZE, (5 * 1024 * 1024, 5 * 1024 * 1024))

def simulate_sketch(code: str):
    build_dir = tempfile.mkdtemp(prefix="tahakom_sim_")
    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sim_bin = os.path.join(build_dir, "sim_app")

    try:
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

        # Stage 1: Compilation
        compile_proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=10)
        if compile_proc.returncode != 0:
            err = compile_proc.stderr[:MAX_OUTPUT_LEN]
            return {
                "success": False,
                "error": f"Compilation failed:\n{err}"
            }

        # Stage 2: Sandboxed Execution
        proc = subprocess.Popen(
            [sim_bin],
            cwd=build_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            preexec_fn=set_sandbox_limits,
            text=True
        )

        try:
            stdout, stderr = proc.communicate(timeout=2.0)
            if proc.returncode != 0:
                return {
                    "success": False,
                    "error": f"Simulation terminated abnormally (Exit code {proc.returncode})"
                }
            return {
                "success": True,
                "stdout": stdout[:MAX_OUTPUT_LEN],
                "stderr": stderr[:MAX_OUTPUT_LEN]
            }

        except subprocess.TimeoutExpired:
            # Terminate the entire process group
            try:
                os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            except Exception:
                proc.kill()
            return {
                "success": False,
                "error": "Simulation execution terminated due to timeout/CPU limit"
            }

    finally:
        # Guarantee cleanup of temporary directory
        shutil.rmtree(build_dir, ignore_errors=True)

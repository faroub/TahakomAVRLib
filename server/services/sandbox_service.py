import os
import signal
import shutil
import threading
import subprocess
import tempfile

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
HOST_DIR = os.path.join(PROJECT_ROOT, "tests/host")
MAX_OUTPUT_LEN = 16 * 1024  # 16 KB max stdout/stderr

_sandbox_available = None

def is_sandbox_available() -> bool:
    """Check whether OS-level Bubblewrap sandbox isolation is available."""
    global _sandbox_available
    if _sandbox_available is not None:
        return _sandbox_available

    bwrap_path = shutil.which("bwrap")
    if not bwrap_path:
        _sandbox_available = False
        return False

    try:
        res = subprocess.run(
            [
                bwrap_path,
                "--unshare-all",
                "--ro-bind", "/usr", "/usr",
                "--ro-bind", "/bin", "/bin",
                "--ro-bind", "/lib", "/lib",
                "--ro-bind", "/lib64", "/lib64",
                "--tmpfs", "/tmp",
                "/usr/bin/true"
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=3
        )
        _sandbox_available = (res.returncode == 0)
    except Exception:
        _sandbox_available = False

    return _sandbox_available

def read_stream_bounded(stream, output_list, max_len, kill_event, proc_pid):
    """Read a stream in chunks up to max_len. Kills process tree if limit exceeded."""
    collected = bytearray()
    while not kill_event.is_set():
        chunk = stream.read(1024)
        if not chunk:
            break
        collected.extend(chunk)
        if len(collected) > max_len:
            kill_event.set()
            try:
                os.killpg(os.getpgid(proc_pid), signal.SIGKILL)
            except Exception:
                pass
            break
    output_list.append(bytes(collected[:max_len]).decode("utf-8", errors="replace"))

def simulate_sketch(code: str):
    if not isinstance(code, str) or not code.strip():
        return {"success": False, "error": "Source code is empty"}

    if not is_sandbox_available():
        return {
            "success": False,
            "error": "Fail-closed Security Error: Isolated sandbox environment (bwrap) is unavailable on host."
        }

    # Private temporary directory with safe 0o700 permissions
    build_dir = tempfile.mkdtemp(prefix="tahakom_sim_")
    os.chmod(build_dir, 0o700)

    sketch_cpp = os.path.join(build_dir, "main.cpp")
    sim_bin = os.path.join(build_dir, "sim_app")

    try:
        if "host_registers.h" not in code:
            code = '#include "host_registers.h"\n' + code

        with open(sketch_cpp, "w") as f:
            f.write(code)

        compile_cmd = [
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
        try:
            compile_proc = subprocess.run(compile_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=10)
        except subprocess.TimeoutExpired:
            return {"success": False, "error": "Compilation process timed out after 10s"}

        if compile_proc.returncode != 0:
            err = compile_proc.stderr[:MAX_OUTPUT_LEN]
            return {
                "success": False,
                "error": f"Compilation failed:\n{err}"
            }

        # Stage 2: Sandboxed Execution via Bubblewrap (bwrap)
        bwrap_cmd = [
            "bwrap",
            "--unshare-all",
            "--ro-bind", "/usr", "/usr",
            "--ro-bind", "/bin", "/bin",
            "--ro-bind", "/lib", "/lib",
            "--ro-bind", "/lib64", "/lib64",
            "--proc", "/proc",
            "--dev", "/dev",
            "--tmpfs", "/tmp",
            "--bind", build_dir, build_dir,
            "--chdir", build_dir,
            sim_bin
        ]

        proc = subprocess.Popen(
            bwrap_cmd,
            cwd=build_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            preexec_fn=os.setpgrp
        )

        kill_event = threading.Event()
        stdout_buf = []
        stderr_buf = []

        t_out = threading.Thread(target=read_stream_bounded, args=(proc.stdout, stdout_buf, MAX_OUTPUT_LEN, kill_event, proc.pid))
        t_err = threading.Thread(target=read_stream_bounded, args=(proc.stderr, stderr_buf, MAX_OUTPUT_LEN, kill_event, proc.pid))

        t_out.start()
        t_err.start()

        try:
            proc.wait(timeout=2.0)
        except subprocess.TimeoutExpired:
            kill_event.set()
            try:
                os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            except Exception:
                proc.kill()
            proc.wait()
            t_out.join(timeout=1.0)
            t_err.join(timeout=1.0)
            return {
                "success": False,
                "error": "Simulation execution terminated due to timeout (2.0s limit exceeded)"
            }

        t_out.join(timeout=1.0)
        t_err.join(timeout=1.0)

        proc.stdout.close()
        proc.stderr.close()

        stdout_str = stdout_buf[0] if stdout_buf else ""
        stderr_str = stderr_buf[0] if stderr_buf else ""

        if kill_event.is_set():
            return {
                "success": False,
                "error": "Simulation output limit exceeded (16KB limit)"
            }

        if proc.returncode != 0:
            return {
                "success": False,
                "error": f"Simulation terminated abnormally (Exit code {proc.returncode})"
            }

        return {
            "success": True,
            "stdout": stdout_str,
            "stderr": stderr_str
        }

    finally:
        shutil.rmtree(build_dir, ignore_errors=True)

import os
import glob
import subprocess
import tempfile

def list_serial_ports():
    ports = []
    # Linux serial device nodes
    for p in glob.glob("/dev/ttyACM*") + glob.glob("/dev/ttyUSB*"):
        ports.append(p)
    return ports

def flash_hex(hex_content: str, mcu: str = "atmega328p", programmer: str = "arduino", port: str = None, baud: int = 115200):
    if not port:
        available = list_serial_ports()
        if not available:
            return {"success": False, "error": "No serial port detected"}
        port = available[0]

    with tempfile.NamedTemporaryFile(suffix=".hex", delete=False, mode="w") as f:
        f.write(hex_content)
        hex_path = f.name

    cmd = [
        "avrdude",
        "-p", mcu,
        "-c", programmer,
        "-P", port,
        "-b", str(baud),
        "-U", f"flash:w:{hex_path}:i"
    ]

    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    os.remove(hex_path)

    return {
        "success": proc.returncode == 0,
        "stdout": proc.stdout,
        "stderr": proc.stderr,
        "port": port
    }

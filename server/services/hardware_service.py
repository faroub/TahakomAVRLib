import os
import glob
import subprocess
import tempfile

MCU_FLASH_CAPACITIES = {
    "atmega328p": 32768,
    "atmega2560": 262144,
    "atmega168p": 16384,
    "atmega88p": 8192,
    "atmega48p": 4096,
    "attiny85": 8192
}

ALLOWED_PROGRAMMERS = {"arduino", "stk500v1", "stk500v2", "usbtiny", "usbasp", "wiring", "buspirate"}

def list_serial_ports():
    ports = []
    for p in glob.glob("/dev/ttyACM*") + glob.glob("/dev/ttyUSB*"):
        ports.append(p)
    return ports

def validate_intel_hex(hex_str: str):
    if not hex_str or not hex_str.strip():
        return False, "HEX content is empty", 0

    lines = hex_str.strip().splitlines()
    total_data_bytes = 0
    has_eof = False

    for line_num, line in enumerate(lines, 1):
        line = line.strip()
        if not line:
            continue
        if not line.startswith(":"):
            return False, f"Line {line_num}: IHEX line does not start with ':'", 0

        try:
            line_bytes = bytes.fromhex(line[1:])
        except ValueError:
            return False, f"Line {line_num}: IHEX line contains non-hex characters", 0

        if sum(line_bytes) % 256 != 0:
            return False, f"Line {line_num}: IHEX checksum error", 0

        byte_count = line_bytes[0]
        record_type = line_bytes[3]

        if record_type == 0:
            total_data_bytes += byte_count
        elif record_type == 1:
            has_eof = True

    if not has_eof:
        return False, "Missing IHEX End-Of-File record (:00000001FF)", 0

    return True, "Valid Intel HEX", total_data_bytes

def flash_hex(hex_content: str, mcu: str = "atmega328p", programmer: str = "arduino", port: str = None, baud: int = 115200):
    mcu = mcu.lower()
    if mcu not in MCU_FLASH_CAPACITIES:
        return {"success": False, "error": f"Unsupported MCU: '{mcu}'"}

    if programmer.lower() not in ALLOWED_PROGRAMMERS:
        return {"success": False, "error": f"Unsupported programmer type: '{programmer}'"}

    is_valid, msg, hex_size = validate_intel_hex(hex_content)
    if not is_valid:
        return {"success": False, "error": f"Invalid Intel HEX: {msg}"}

    max_capacity = MCU_FLASH_CAPACITIES[mcu]
    if hex_size > max_capacity:
        return {
            "success": False,
            "error": f"Firmware size ({hex_size} bytes) exceeds {mcu} flash capacity ({max_capacity} bytes)"
        }

    available_ports = list_serial_ports()
    if port:
        if port not in available_ports and not (port.startswith("/dev/ttyACM") or port.startswith("/dev/ttyUSB")):
            return {"success": False, "error": f"Invalid or non-permitted serial port: '{port}'"}
    else:
        if not available_ports:
            return {"success": False, "error": "No serial port detected on host"}
        port = available_ports[0]

    hex_path = None
    try:
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

        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=30)
        return {
            "success": proc.returncode == 0,
            "stdout": proc.stdout[:8192],
            "stderr": proc.stderr[:8192],
            "port": port,
            "hex_size": hex_size
        }
    except subprocess.TimeoutExpired:
        return {"success": False, "error": "Flashing process timed out"}
    finally:
        if hex_path and os.path.exists(hex_path):
            os.remove(hex_path)

import os
import glob
import signal
import threading
import subprocess
import tempfile

MCU_FLASH_CAPACITIES = {
    "atmega328p": 32768,
    "atmega2560": 262144,
    "atmega168p": 16384,
    "atmega88p": 8192,
    "atmega48p": 4096,
    "attiny85": 8192,
    "esp32": 4194304,
    "esp32s3": 8388608
}

ALLOWED_PROGRAMMERS = {"arduino", "stk500v1", "stk500v2", "usbtiny", "usbasp", "wiring", "buspirate", "esptool"}

# MCU to Programmer explicit compatibility matrix
MCU_PROGRAMMER_MATRIX = {
    "atmega328p": {"arduino", "stk500v1", "stk500v2", "usbtiny", "usbasp", "buspirate"},
    "atmega2560": {"wiring", "stk500v2", "usbtiny", "usbasp", "buspirate"},
    "atmega168p": {"arduino", "stk500v1", "usbtiny", "usbasp"},
    "atmega88p":  {"arduino", "stk500v1", "usbtiny", "usbasp"},
    "atmega48p":  {"arduino", "stk500v1", "usbtiny", "usbasp"},
    "attiny85":   {"usbtiny", "usbasp", "stk500v1"},
    "esp32":      {"esptool"},
    "esp32s3":    {"esptool"}
}

MAX_HEX_SIZE = 1024 * 1024  # 1MB max string
MAX_HEX_LINES = 100000
MAX_OUTPUT_LEN = 16 * 1024

def list_serial_ports():
    ports = []
    for p in glob.glob("/dev/ttyACM*") + glob.glob("/dev/ttyUSB*"):
        ports.append(p)
    return ports

def validate_intel_hex(hex_str: str, mcu: str = "atmega328p"):
    if not isinstance(hex_str, str) or not hex_str.strip():
        return False, "HEX content is empty", 0

    if len(hex_str) > MAX_HEX_SIZE:
        return False, f"HEX payload size exceeds {MAX_HEX_SIZE} bytes limit", 0

    mcu_key = mcu.lower().strip() if isinstance(mcu, str) else "atmega328p"
    max_capacity = MCU_FLASH_CAPACITIES.get(mcu_key, 32768)

    lines = hex_str.strip().splitlines()
    if len(lines) > MAX_HEX_LINES:
        return False, f"HEX line count exceeds {MAX_HEX_LINES} lines limit", 0

    base_addr = 0
    memory_map = {}
    seen_eof = False

    for line_num, raw_line in enumerate(lines, 1):
        line = raw_line.strip()
        if not line:
            continue

        if seen_eof:
            return False, f"Line {line_num}: Unpermitted data following End-Of-File record", 0

        if not line.startswith(":"):
            return False, f"Line {line_num}: IHEX line does not start with ':'", 0

        if len(line) < 11:
            return False, f"Line {line_num}: Incomplete IHEX record (too short)", 0

        try:
            record_bytes = bytes.fromhex(line[1:])
        except ValueError:
            return False, f"Line {line_num}: IHEX line contains non-hex characters", 0

        byte_count = record_bytes[0]
        rec_addr = (record_bytes[1] << 8) | record_bytes[2]
        rec_type = record_bytes[3]
        payload = record_bytes[4:-1]

        if len(line) != 11 + byte_count * 2:
            return False, f"Line {line_num}: Record length mismatch", 0

        if len(payload) != byte_count:
            return False, f"Line {line_num}: Data payload byte count mismatch", 0

        if sum(record_bytes) % 256 != 0:
            return False, f"Line {line_num}: IHEX checksum error", 0

        if rec_type == 0:  # Data Record
            for i, val in enumerate(payload):
                abs_addr = base_addr + rec_addr + i
                if abs_addr >= max_capacity or abs_addr < 0:
                    return False, f"Line {line_num}: Absolute flash address 0x{abs_addr:06X} exceeds {mcu_key} flash capacity ({max_capacity} bytes)", 0
                if abs_addr in memory_map and memory_map[abs_addr] != val:
                    return False, f"Line {line_num}: Conflicting data overlap at address 0x{abs_addr:06X}", 0
                memory_map[abs_addr] = val

        elif rec_type == 1:  # End of File Record
            if byte_count != 0 or rec_addr != 0:
                return False, f"Line {line_num}: Malformed End-Of-File record", 0
            seen_eof = True

        elif rec_type == 2:  # Extended Segment Address Record
            if byte_count != 2:
                return False, f"Line {line_num}: Extended Segment Address record must have length 2", 0
            base_addr = ((payload[0] << 8) | payload[1]) << 4

        elif rec_type == 4:  # Extended Linear Address Record
            if byte_count != 2:
                return False, f"Line {line_num}: Extended Linear Address record must have length 2", 0
            base_addr = ((payload[0] << 8) | payload[1]) << 16

        elif rec_type in (3, 5):  # Start Address Record
            if byte_count != 4:
                return False, f"Line {line_num}: Start Address record must have length 4", 0

        else:
            return False, f"Line {line_num}: Unsupported or unknown IHEX record type 0x{rec_type:02X}", 0

    if not seen_eof:
        return False, "Missing IHEX End-Of-File record (:00000001FF)", 0

    return True, "Valid Intel HEX", len(memory_map)

def flash_hex(hex_content: str, mcu: str = "atmega328p", programmer: str = "arduino", port: str = None, baud: int = 115200, confirmed: bool = False):
    if not confirmed:
        return {"success": False, "error": "Explicit user confirmation (confirmed: true) is required before flashing physical hardware."}

    mcu = mcu.lower().strip() if isinstance(mcu, str) else ""
    if mcu not in MCU_FLASH_CAPACITIES:
        return {"success": False, "error": f"Unsupported or unknown MCU: '{mcu}'"}

    programmer = programmer.lower().strip() if isinstance(programmer, str) else ""
    if programmer not in ALLOWED_PROGRAMMERS:
        return {"success": False, "error": f"Unsupported programmer type: '{programmer}'"}

    # Validate MCU / Programmer compatibility matrix
    permitted_programmers = MCU_PROGRAMMER_MATRIX.get(mcu, set())
    if programmer not in permitted_programmers:
        return {
            "success": False,
            "error": f"Incompatible programmer '{programmer}' for MCU target '{mcu}'. Permitted: {sorted(list(permitted_programmers))}"
        }

    # Strict Intel HEX Validation
    is_valid, msg, hex_size = validate_intel_hex(hex_content, mcu=mcu)
    if not is_valid:
        return {"success": False, "error": f"Invalid Intel HEX: {msg}"}

    # Strict Port Requirement (Prompt 05)
    if not port or not isinstance(port, str):
        return {"success": False, "error": "Explicit serial port selection required. Auto-selection disabled for hardware safety."}

    available_ports = list_serial_ports()
    if port not in available_ports and not (port.startswith("/dev/ttyACM") or port.startswith("/dev/ttyUSB")):
        return {"success": False, "error": f"Invalid or non-permitted serial port: '{port}'"}

    hex_path = None
    try:
        with tempfile.NamedTemporaryFile(suffix=".hex", delete=False, mode="w") as f:
            f.write(hex_content)
            hex_path = f.name
        os.chmod(hex_path, 0o600)

        if mcu in ("esp32", "esp32s3"):
            cmd = ["esptool.py", "--chip", mcu, "--port", port, "--baud", str(baud), "write_flash", "0x10000", hex_path]
        else:
            cmd = ["avrdude", "-p", mcu, "-c", programmer, "-P", port, "-b", str(baud), "-U", f"flash:w:{hex_path}:i"]

        try:
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, preexec_fn=os.setpgrp)
        except FileNotFoundError:
            return {"success": False, "error": f"Flashing tool executable '{cmd[0]}' is not installed on host system PATH."}
        kill_event = threading.Event()
        stdout_buf, stderr_buf = [], []

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
            proc.wait(timeout=30.0)
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

        if timed_out:
            return {"success": False, "error": "Flashing process timed out after 30s"}

        stdout_str = stdout_buf[0] if stdout_buf else ""
        stderr_str = stderr_buf[0] if stderr_buf else ""

        return {
            "success": proc.returncode == 0,
            "stdout": stdout_str,
            "stderr": stderr_str,
            "port": port,
            "hex_size": hex_size
        }
    finally:
        if hex_path and os.path.exists(hex_path):
            os.remove(hex_path)

MAX_TELEMETRY_LINE_LEN = 128

def parse_telemetry_line(line: str):
    if not isinstance(line, str):
        return None

    line = line.strip()
    if len(line) == 0 or len(line) > MAX_TELEMETRY_LINE_LEN:
        return None

    if not line.startswith("$"):
        return None

    parts = line[1:].split(",")
    if not parts or not parts[0]:
        return None

    frame_type = parts[0].upper()

    try:
        if frame_type == "ADC":
            if len(parts) != 3:
                return None
            channel = int(parts[1])
            val = int(parts[2])
            if not (0 <= channel <= 15 and 0 <= val <= 65535):
                return None
            return {"type": "adc", "channel": channel, "value": val}

        elif frame_type == "PIN":
            if len(parts) != 4:
                return None
            port = int(parts[1])
            pin = int(parts[2])
            state = int(parts[3])
            if not (0 <= port <= 2 and 0 <= pin <= 7 and state in (0, 1)):
                return None
            return {"type": "pin", "port": port, "pin": pin, "state": state}

        elif frame_type == "MOTOR":
            if len(parts) != 3:
                return None
            pos = int(parts[1])
            speed = int(parts[2])
            if not (-32768 <= pos <= 65535 and 0 <= speed <= 65535):
                return None
            return {"type": "motor", "position": pos, "speed": speed}

        elif frame_type == "LOG":
            if len(parts) < 2:
                return None
            msg = ",".join(parts[1:]).strip()
            return {"type": "log", "message": msg[:256]}

    except ValueError:
        return None

    return None

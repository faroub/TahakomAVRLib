def parse_telemetry_line(line: str):
    line = line.strip()
    if not line.startswith("$"):
        return None

    parts = line[1:].split(",")
    if not parts:
        return None

    frame_type = parts[0]
    if frame_type == "ADC" and len(parts) >= 3:
        try:
            return {
                "type": "adc",
                "channel": int(parts[1]),
                "value": int(parts[2])
            }
        except ValueError:
            return None
    elif frame_type == "PIN" and len(parts) >= 4:
        try:
            return {
                "type": "pin",
                "port": int(parts[1]),
                "pin": int(parts[2]),
                "state": int(parts[3])
            }
        except ValueError:
            return None
    elif frame_type == "MOTOR" and len(parts) >= 3:
        try:
            return {
                "type": "motor",
                "position": int(parts[1]),
                "speed": int(parts[2])
            }
        except ValueError:
            return None
    elif frame_type == "LOG" and len(parts) >= 2:
        return {
            "type": "log",
            "message": ",".join(parts[1:])
        }

    return None

import os
import json
import time
import logging
import threading

LOG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), "../audit.log"))
REDACTED_KEYS = {"code", "hex", "token", "x-auth-token", "password", "secret"}

class AuditLogger:
    def __init__(self, log_path: str = LOG_FILE, max_bytes: int = 5 * 1024 * 1024, backup_count: int = 3):
        self.log_path = log_path
        self.max_bytes = max_bytes
        self.backup_count = backup_count
        self.lock = threading.Lock()

    def _sanitize_details(self, details: dict) -> dict:
        if not isinstance(details, dict):
            return {}
        sanitized = {}
        for k, v in details.items():
            if k.lower() in REDACTED_KEYS:
                sanitized[k] = "[REDACTED]"
            elif isinstance(v, str) and len(v) > 256:
                sanitized[k] = v[:256] + "...[TRUNCATED]"
            else:
                sanitized[k] = v
        return sanitized

    def _rotate_if_needed(self):
        if not os.path.exists(self.log_path):
            return

        if os.path.getsize(self.log_path) < self.max_bytes:
            return

        for i in range(self.backup_count - 1, 0, -1):
            sfn = f"{self.log_path}.{i}"
            dfn = f"{self.log_path}.{i + 1}"
            if os.path.exists(sfn):
                if os.path.exists(dfn):
                    os.remove(dfn)
                os.rename(sfn, dfn)

        dfn = f"{self.log_path}.1"
        if os.path.exists(dfn):
            os.remove(dfn)
        os.rename(self.log_path, dfn)

    def log_event(self, client_ip: str, endpoint: str, action: str, status: str, details: dict = None):
        with self.lock:
            try:
                self._rotate_if_needed()

                record = {
                    "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
                    "client_ip": client_ip,
                    "endpoint": endpoint,
                    "action": action,
                    "status": status,
                    "details": self._sanitize_details(details)
                }

                log_line = json.dumps(record) + "\n"

                flags = os.O_WRONLY | os.O_CREAT | os.O_APPEND
                fd = os.open(self.log_path, flags, 0o600)
                try:
                    with open(fd, "a", closefd=False) as f:
                        f.write(log_line)
                finally:
                    os.close(fd)

            except Exception as e:
                logging.error("Failed to write audit log: %s", e)

# Global instance
audit_logger = AuditLogger()

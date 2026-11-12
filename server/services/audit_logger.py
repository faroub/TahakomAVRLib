import os
import json
import time
import logging

LOG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), "../audit.log"))

class AuditLogger:
    def __init__(self, log_path: str = LOG_FILE):
        self.log_path = log_path

    def log_event(self, client_ip: str, endpoint: str, action: str, status: str, details: dict = None):
        record = {
            "timestamp": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
            "client_ip": client_ip,
            "endpoint": endpoint,
            "action": action,
            "status": status,
            "details": details or {}
        }

        log_line = json.dumps(record) + "\n"
        try:
            with open(self.log_path, "a") as f:
                f.write(log_line)
        except Exception as e:
            logging.error("Failed to write audit log: %s", e)

# Global instance
audit_logger = AuditLogger()

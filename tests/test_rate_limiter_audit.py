import unittest
import os
import sys
import tempfile
import json

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.rate_limiter import SlidingWindowRateLimiter
from services.audit_logger import AuditLogger

class TestRateLimiterAudit(unittest.TestCase):

    def test_rate_limiter_ip_pruning_and_bounds(self):
        limiter = SlidingWindowRateLimiter(max_requests=10, window_seconds=5, max_tracked_ips=100)
        for i in range(150):
            limiter.is_allowed(f"192.168.1.{i}")
        self.assertLessEqual(len(limiter.requests), 100)

    def test_audit_logger_redaction_and_rotation(self):
        tmp_dir = tempfile.mkdtemp()
        log_path = os.path.join(tmp_dir, "test_audit.log")
        logger = AuditLogger(log_path=log_path, max_bytes=1000, backup_count=2)

        # Log event with sensitive fields
        details = {
            "code": "#include <stdio.h> int main(){}",
            "hex": ":10000000...",
            "token": "secret_token_123",
            "mcu": "atmega328p"
        }
        logger.log_event("127.0.0.1", "/api/compile", "BUILD", "SUCCESS", details)

        with open(log_path, "r") as f:
            content = f.read()

        self.assertNotIn("secret_token_123", content)
        self.assertNotIn("#include <stdio.h>", content)
        self.assertIn("[REDACTED]", content)
        self.assertIn("atmega328p", content)

        # Force log rotation by filling file > 1000 bytes
        for i in range(50):
            logger.log_event("127.0.0.1", "/api/compile", "BUILD", "SUCCESS", {"mcu": f"mcu_{i}"})

        self.assertTrue(os.path.exists(log_path + ".1"))

if __name__ == "__main__":
    unittest.main()

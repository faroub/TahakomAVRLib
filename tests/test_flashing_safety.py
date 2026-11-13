import unittest
import os
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.hardware_service import flash_hex

class TestFlashingSafety(unittest.TestCase):

    def setUp(self):
        self.valid_hex = (
            ":100000000C9434000C9451000C9451000C94510049\n"
            ":00000001FF\n"
        )

    def test_unconfirmed_flash_rejected(self):
        # Flashing without confirmed=True MUST be rejected
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", confirmed=False)
        self.assertFalse(res["success"])
        self.assertIn("confirmation", res["error"].lower())

    def test_missing_port_rejected(self):
        # Auto port selection (port=None) MUST be rejected
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port=None, confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("port", res["error"].lower())

    def test_arbitrary_invalid_port_path_rejected(self):
        # Arbitrary non-serial path must be rejected
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/etc/passwd", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("port", res["error"].lower())

    def test_incompatible_programmer_mcu_matrix_rejected(self):
        # arduino programmer is incompatible with atmega2560 (which requires wiring)
        res = flash_hex(self.valid_hex, mcu="atmega2560", programmer="arduino", port="/dev/ttyACM0", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("incompatible", res["error"].lower())

    def test_esp32_programmer_matrix_enforced(self):
        # avrdude programmers (stk500v1) must be rejected for esp32
        res = flash_hex(self.valid_hex, mcu="esp32", programmer="stk500v1", port="/dev/ttyUSB0", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("incompatible", res["error"].lower())

if __name__ == "__main__":
    unittest.main()

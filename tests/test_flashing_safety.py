import unittest
import os
import sys
from unittest.mock import patch, MagicMock

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
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", confirmed=False)
        self.assertFalse(res["success"])
        self.assertIn("confirmation", res["error"].lower())

    def test_missing_port_rejected(self):
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port=None, confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("port", res["error"].lower())

    def test_traversal_path_port_rejected(self):
        # /dev/ttyUSB0/../../../../tmp/evil MUST be rejected
        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0/../../../../tmp/evil", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("port", res["error"].lower())

    def test_nonexistent_port_rejected(self):
        # /dev/ttyUSB9 (not in list_serial_ports()) MUST be rejected
        with patch("services.hardware_service.list_serial_ports", return_value=["/dev/ttyUSB0"]):
            res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB9", confirmed=True)
            self.assertFalse(res["success"])
            self.assertIn("non-existent", res["error"].lower())

    @patch("services.hardware_service.subprocess.Popen")
    @patch("services.hardware_service.list_serial_ports")
    def test_valid_port_in_mocked_list_accepted_up_to_subprocess(self, mock_list_ports, mock_popen):
        mock_list_ports.return_value = ["/dev/ttyUSB0"]

        mock_proc = MagicMock()
        mock_proc.returncode = 0
        mock_proc.stdout.read.return_value = b"avrdude done. Thank you."
        mock_proc.stderr.read.return_value = b""
        mock_proc.wait.return_value = 0
        mock_popen.return_value = mock_proc

        res = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", confirmed=True)
        self.assertTrue(res["success"])
        self.assertEqual(res["port"], "/dev/ttyUSB0")
        mock_popen.assert_called_once()

    def test_invalid_baud_rate_rejected(self):
        # Out-of-range baud rate
        res1 = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", baud=100, confirmed=True)
        self.assertFalse(res1["success"])
        self.assertIn("baud", res1["error"].lower())

        res2 = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", baud=10000000, confirmed=True)
        self.assertFalse(res2["success"])
        self.assertIn("baud", res2["error"].lower())

        res3 = flash_hex(self.valid_hex, mcu="atmega328p", programmer="arduino", port="/dev/ttyUSB0", baud="invalid", confirmed=True)
        self.assertFalse(res3["success"])
        self.assertIn("baud", res3["error"].lower())

    def test_incompatible_programmer_mcu_matrix_rejected(self):
        res = flash_hex(self.valid_hex, mcu="atmega2560", programmer="arduino", port="/dev/ttyACM0", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("incompatible", res["error"].lower())

    def test_esp32_programmer_matrix_enforced(self):
        res = flash_hex(self.valid_hex, mcu="esp32", programmer="stk500v1", port="/dev/ttyUSB0", confirmed=True)
        self.assertFalse(res["success"])
        self.assertIn("incompatible", res["error"].lower())

if __name__ == "__main__":
    unittest.main()

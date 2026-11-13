import unittest
import os
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.build_service import compile_sketch, is_esp32_toolchain_available
from services.hardware_service import flash_hex

class TestTargetBackends(unittest.TestCase):

    def test_avr_target_uses_avr_toolchain(self):
        code = """
#include "Led.h"
#include "MCU.h"
int main(void) {
    core::MCU::init();
    return 0;
}
        """
        res = compile_sketch(code, mcu="atmega328p")
        self.assertTrue(res["success"])
        self.assertIsNotNone(res["hex"])

    def test_esp32_target_without_toolchain_fails_validation(self):
        # If ESP32 toolchain is unavailable, must return explicit validation error, NOT pass esp32 to avr-g++
        code = "void setup(){} void loop(){}"
        if not is_esp32_toolchain_available():
            res = compile_sketch(code, mcu="esp32")
            self.assertFalse(res["success"])
            self.assertEqual(res["stage"], "validation")
            self.assertIn("toolchain", res["error"].lower())

    def test_esp32_flashing_without_esptool_or_device_fails_gracefully(self):
        hex_content = ":00000001FF\n"
        res = flash_hex(hex_content, mcu="esp32", programmer="esptool", port="/dev/ttyUSB0", confirmed=True)
        # Should fail cleanly if esptool or hardware is unavailable
        self.assertFalse(res["success"])

if __name__ == "__main__":
    unittest.main()

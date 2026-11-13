import unittest
import os
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.build_service import compile_sketch

class TestBuildResourceSafety(unittest.TestCase):

    def test_boolean_and_non_int_parameters_rejected(self):
        code = "int main(void) { return 0; }"
        # Boolean f_cpu
        res1 = compile_sketch(code, f_cpu=True)
        self.assertFalse(res1["success"])
        self.assertEqual(res1["stage"], "validation")

        # Float f_cpu
        res2 = compile_sketch(code, f_cpu=16000000.5)
        self.assertFalse(res2["success"])
        self.assertEqual(res2["stage"], "validation")

        # Negative baud
        res3 = compile_sketch(code, baud=-9600)
        self.assertFalse(res3["success"])
        self.assertEqual(res3["stage"], "validation")

    def test_oversized_code_rejected(self):
        code = "a" * (512 * 1024 + 1)
        res = compile_sketch(code)
        self.assertFalse(res["success"])
        self.assertEqual(res["stage"], "validation")

    def test_unknown_mcu_rejected(self):
        code = "int main(void) { return 0; }"
        res = compile_sketch(code, mcu="unknown_chip_xyz")
        self.assertFalse(res["success"])
        self.assertEqual(res["stage"], "validation")

    def test_successful_compilation_cleans_temp_dir(self):
        code = """
#include "Led.h"
#include "MCU.h"
int main(void) {
    core::MCU::init();
    component::Led led(io::Pin(0, io::PortB));
    led.on();
    return 0;
}
        """
        res = compile_sketch(code, mcu="atmega328p")
        self.assertTrue(res["success"], f"Compilation failed: {res}")
        self.assertIsNotNone(res["hex"])

if __name__ == "__main__":
    unittest.main()

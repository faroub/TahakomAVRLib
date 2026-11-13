import unittest
import os
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.hardware_service import validate_intel_hex

class TestIntelHexStrict(unittest.TestCase):

    def test_valid_ihex_passes(self):
        # Line 1 sum = 951, checksum 0x49 -> (951 + 73) % 256 == 0
        # Line 2 sum = 951, addr 0x0010 (16), total sum = 967, checksum 0x39 -> (967 + 57) % 256 == 0
        hex_content = (
            ":100000000C9434000C9451000C9451000C94510049\n"
            ":100010000C9434000C9451000C9451000C94510039\n"
            ":00000001FF\n"
        )
        valid, msg, bytes_cnt = validate_intel_hex(hex_content, mcu="atmega328p")
        self.assertTrue(valid, f"Valid IHEX failed: {msg}")
        self.assertEqual(bytes_cnt, 32)

    def test_short_record_no_index_error(self):
        short_hex = ":10\n:00000001FF\n"
        valid, msg, _ = validate_intel_hex(short_hex, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("Line 1", msg)

    def test_incomplete_colon_line(self):
        bad_hex = ":\n:00000001FF\n"
        valid, msg, _ = validate_intel_hex(bad_hex, mcu="atmega328p")
        self.assertFalse(valid)

    def test_checksum_error_rejected(self):
        bad_cksum = ":100000000C9434000C9451000C9451000C94510000\n:00000001FF\n"
        valid, msg, _ = validate_intel_hex(bad_cksum, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("checksum", msg.lower())

    def test_missing_eof_rejected(self):
        no_eof = ":100000000C9434000C9451000C9451000C94510049\n"
        valid, msg, _ = validate_intel_hex(no_eof, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("end-of-file", msg.lower())

    def test_data_after_eof_rejected(self):
        data_after_eof = (
            ":100000000C9434000C9451000C9451000C94510049\n"
            ":00000001FF\n"
            ":100010000C9434000C9451000C9451000C94510039\n"
        )
        valid, msg, _ = validate_intel_hex(data_after_eof, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("end-of-file", msg.lower())

    def test_out_of_range_address_rejected(self):
        # ATmega328P flash is 32768 bytes (0x0000 to 0x7FFF). Address 0x8000 (32768) is out of range.
        # Sum for :108000000C9434000C9451000C9451000C945100 -> 16+128+0+0+935 = 1079. 1079 & 0xFF = 0x37. Checksum = 0xC9.
        out_of_range = (
            ":108000000C9434000C9451000C9451000C945100C9\n"
            ":00000001FF\n"
        )
        valid, msg, _ = validate_intel_hex(out_of_range, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("capacity", msg.lower())

    def test_conflicting_overlap_rejected(self):
        # Two records writing different values to address 0x0000
        # Line 1: :01000000AA4B -> sum = 1 + 0 + 0 + 0 + 170 + 75 = 246 (Wait: 1+0+0+0+170 = 171. Two's comp of 171 is 0x55 = 85. 171+85 = 256 % 256 == 0)
        # Line 2: :01000000BB44 -> sum = 1 + 0 + 0 + 0 + 187 = 188. Two's comp of 188 is 0x44 = 68. 188+68 = 256 % 256 == 0
        overlap = (
            ":01000000AA55\n"
            ":01000000BB44\n"
            ":00000001FF\n"
        )
        valid, msg, _ = validate_intel_hex(overlap, mcu="atmega328p")
        self.assertFalse(valid)
        self.assertIn("overlap", msg.lower())

    def test_extended_linear_address_record(self):
        # Type 04 Extended Linear Address (0x0000)
        ext_addr = (
            ":020000040000FA\n"
            ":01000000AA55\n"
            ":00000001FF\n"
        )
        valid, msg, bytes_cnt = validate_intel_hex(ext_addr, mcu="atmega328p")
        self.assertTrue(valid, f"Extended linear address failed: {msg}")
        self.assertEqual(bytes_cnt, 1)

if __name__ == "__main__":
    unittest.main()

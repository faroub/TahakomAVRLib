import unittest
import os
import subprocess

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
CHECK_BUILD_SCRIPT = os.path.join(PROJECT_ROOT, "scripts/check_build.sh")

class TestCIBaselineSafety(unittest.TestCase):

    def test_check_build_passes_cleanly(self):
        res = subprocess.run([CHECK_BUILD_SCRIPT], cwd=PROJECT_ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.assertEqual(res.returncode, 0, f"check_build.sh failed: {res.stderr}\n{res.stdout}")
        self.assertIn("OK: no errors", res.stdout)

if __name__ == "__main__":
    unittest.main()

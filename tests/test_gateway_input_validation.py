import unittest
import os
import sys
import json
import time
import urllib.request
import urllib.error
import subprocess

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_APP = os.path.join(PROJECT_ROOT, "server/app.py")
PORT = 8399
BASE_URL = f"http://127.0.0.1:{PORT}"
AUTH_TOKEN = "validation_test_token_555"

def wait_for_server(timeout=5.0):
    start = time.time()
    while time.time() - start < timeout:
        try:
            req = urllib.request.Request(f"{BASE_URL}/api/token", headers={"X-Auth-Token": AUTH_TOKEN})
            with urllib.request.urlopen(req) as resp:
                if resp.status == 200:
                    return True
        except Exception:
            time.sleep(0.1)
    return False

class TestGatewayInputValidation(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        env = os.environ.copy()
        env["PORT"] = str(PORT)
        env["TAHAKOM_AUTH_TOKEN"] = AUTH_TOKEN
        cls.server_proc = subprocess.Popen([sys.executable, SERVER_APP], cwd=PROJECT_ROOT, env=env)
        if not wait_for_server():
            cls.server_proc.terminate()
            raise RuntimeError("Test server failed to start")

    @classmethod
    def tearDownClass(cls):
        cls.server_proc.terminate()
        cls.server_proc.wait()

    def test_boolean_f_cpu_rejected(self):
        payload = json.dumps({"code": "int main(){}", "mcu": "atmega328p", "f_cpu": True}).encode("utf-8")
        req = urllib.request.Request(f"{BASE_URL}/api/compile", data=payload, headers={"Content-Type": "application/json", "X-Auth-Token": AUTH_TOKEN})
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("Boolean f_cpu should be rejected with 400")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 400)

    def test_unknown_endpoint_returns_404(self):
        req = urllib.request.Request(f"{BASE_URL}/api/unknown_endpoint", headers={"X-Auth-Token": AUTH_TOKEN})
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("Unknown endpoint should return 404")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 404)

    def test_unsupported_method_returns_405(self):
        req = urllib.request.Request(f"{BASE_URL}/api/compile", method="DELETE", headers={"X-Auth-Token": AUTH_TOKEN})
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("DELETE method should return 405")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 405)

    def test_non_dict_json_payloads_rejected(self):
        bad_payloads = [
            json.dumps("just a string").encode("utf-8"),
            json.dumps(12345).encode("utf-8"),
            json.dumps(True).encode("utf-8")
        ]
        for payload in bad_payloads:
            req = urllib.request.Request(f"{BASE_URL}/api/compile", data=payload, headers={"Content-Type": "application/json", "X-Auth-Token": AUTH_TOKEN})
            try:
                with urllib.request.urlopen(req) as resp:
                    self.fail("Non-dict JSON payload should be rejected with 400")
            except urllib.error.HTTPError as e:
                self.assertEqual(e.code, 400)

if __name__ == "__main__":
    unittest.main()

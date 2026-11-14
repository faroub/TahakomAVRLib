import unittest
import os
import sys
import json
import time
import socket
import urllib.request
import urllib.error
import subprocess

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_APP = os.path.join(PROJECT_ROOT, "server/app.py")
PORT = 8299
BASE_URL = f"http://127.0.0.1:{PORT}"
AUTH_TOKEN = "security_test_token_98765"

def wait_for_server(timeout=5.0):
    start = time.time()
    while time.time() - start < timeout:
        try:
            req = urllib.request.Request(f"{BASE_URL}/api/token", headers={"X-Auth-Token": AUTH_TOKEN, "Host": f"127.0.0.1:{PORT}"})
            with urllib.request.urlopen(req) as resp:
                if resp.status == 200:
                    return True
        except Exception:
            time.sleep(0.1)
    return False

class TestGatewaySecurity(unittest.TestCase):
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

    def test_foreign_host_header_rejected(self):
        # Host: attacker.example MUST return 403
        req = urllib.request.Request(
            f"{BASE_URL}/api/token",
            headers={"Host": "attacker.example", "X-Auth-Token": AUTH_TOKEN}
        )
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("Foreign Host header should be rejected with 403")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 403)

    def test_foreign_origin_rejected(self):
        # Origin: http://attacker.example MUST return 403
        req = urllib.request.Request(
            f"{BASE_URL}/api/token",
            headers={"Origin": "http://attacker.example", "Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN}
        )
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("Foreign Origin header should be rejected with 403")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 403)

    def test_non_boolean_confirmed_rejected(self):
        # confirmed = 1 MUST be rejected (must be literal boolean True)
        payload = json.dumps({
            "hex": ":00000001FF\n",
            "mcu": "atmega328p",
            "programmer": "arduino",
            "port": "/dev/ttyUSB0",
            "confirmed": 1
        }).encode("utf-8")
        req = urllib.request.Request(
            f"{BASE_URL}/api/flash",
            data=payload,
            headers={"Content-Type": "application/json", "Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN}
        )
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("non-boolean confirmed=1 should be rejected with 400")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 400)

    def test_hostile_origin_substring_attack_rejected(self):
        hostile_origins = [
            "http://localhost.attacker.example",
            "http://127.0.0.1.attacker.example",
            "http://attacker.example/localhost",
            "http://user:pass@localhost:8299",
            "ftp://localhost:8299",
            "http://localhost:9999"
        ]
        for origin in hostile_origins:
            req = urllib.request.Request(
                f"{BASE_URL}/api/token",
                headers={"Origin": origin, "Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN}
            )
            try:
                with urllib.request.urlopen(req) as resp:
                    self.fail(f"Hostile origin '{origin}' should be rejected with 403")
            except urllib.error.HTTPError as e:
                self.assertEqual(e.code, 403)

    def test_valid_origin_allowed(self):
        valid_origins = [
            f"http://localhost:{PORT}",
            f"http://127.0.0.1:{PORT}"
        ]
        for origin in valid_origins:
            req = urllib.request.Request(
                f"{BASE_URL}/api/token",
                headers={"Origin": origin, "Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN}
            )
            with urllib.request.urlopen(req) as resp:
                headers = dict(resp.info())
                self.assertEqual(headers.get("Access-Control-Allow-Origin"), origin)

    def test_unauthenticated_api_ports_rejected(self):
        req = urllib.request.Request(f"{BASE_URL}/api/ports", headers={"Host": f"127.0.0.1:{PORT}"})
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("/api/ports should require authentication")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 401)

    def test_authenticated_api_ports_succeeds(self):
        req = urllib.request.Request(f"{BASE_URL}/api/ports", headers={"Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN})
        with urllib.request.urlopen(req) as resp:
            self.assertEqual(resp.status, 200)
            data = json.loads(resp.read().decode("utf-8"))
            self.assertIn("ports", data)

    def test_negative_content_length_rejected(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", PORT))
        raw_req = (
            f"POST /api/compile HTTP/1.1\r\n"
            f"Host: 127.0.0.1:{PORT}\r\n"
            f"X-Auth-Token: {AUTH_TOKEN}\r\n"
            f"Content-Type: application/json\r\n"
            f"Content-Length: -10\r\n\r\n"
        ).encode("utf-8")
        s.sendall(raw_req)
        resp_data = s.recv(4096).decode("utf-8", errors="ignore")
        s.close()
        self.assertTrue("400 Bad Request" in resp_data or "411 Length Required" in resp_data or "413 Payload" in resp_data)

    def test_top_level_json_array_rejected(self):
        payload = json.dumps([1, 2, 3]).encode("utf-8")
        req = urllib.request.Request(
            f"{BASE_URL}/api/compile",
            data=payload,
            headers={"Content-Type": "application/json", "Host": f"127.0.0.1:{PORT}", "X-Auth-Token": AUTH_TOKEN}
        )
        try:
            with urllib.request.urlopen(req) as resp:
                self.fail("JSON array should be rejected")
        except urllib.error.HTTPError as e:
            self.assertEqual(e.code, 400)

if __name__ == "__main__":
    unittest.main()

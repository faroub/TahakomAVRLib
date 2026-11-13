#!/usr/bin/env python3
"""
Server Security & API Smoke Test for TahakomAVR Platform
"""

import sys
import os
import time
import json
import urllib.request
import urllib.error
import subprocess

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_APP = os.path.join(PROJECT_ROOT, "server/app.py")
PORT = 8199
BASE_URL = f"http://localhost:{PORT}"
AUTH_TOKEN = "ci_secret_test_token_1234"

def wait_for_server(timeout=10.0):
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

def run_tests():
    print(f"--- Starting local server on port {PORT} for CI security smoke testing ---")
    env = os.environ.copy()
    env["PORT"] = str(PORT)
    env["TAHAKOM_AUTH_TOKEN"] = AUTH_TOKEN
    proc = subprocess.Popen([sys.executable, SERVER_APP], cwd=PROJECT_ROOT, env=env)

    try:
        if not wait_for_server():
            raise RuntimeError("Server failed to bind and become ready within 10 seconds")

        auth_headers = {
            "Content-Type": "application/json",
            "X-Auth-Token": AUTH_TOKEN
        }

        # Test 1: Authenticated GET /api/ports
        req = urllib.request.Request(f"{BASE_URL}/api/ports", headers={"X-Auth-Token": AUTH_TOKEN})
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  [Pass] Authenticated GET /api/ports ->", data)
            assert "ports" in data

        # Test 2: Unauthorized request (Missing token) -> 401
        sketch_code = """
#include "Led.h"
#include "MCU.h"
int main(void) {
    core::MCU::init();
    component::Led led(io::Pin(0, io::PortB));
    led.on();
    return 0;
}
        """
        payload = json.dumps({"code": sketch_code, "mcu": "atmega328p"}).encode("utf-8")
        unauth_req = urllib.request.Request(f"{BASE_URL}/api/compile", data=payload, headers={"Content-Type": "application/json"})
        try:
            with urllib.request.urlopen(unauth_req) as resp:
                assert False, "Unauthenticated request should have failed with 401"
        except urllib.error.HTTPError as e:
            print(f"  [Pass] Unauthorized request correctly rejected with HTTP {e.code}")
            assert e.code == 401

        # Test 3: Authorized POST /api/compile
        req = urllib.request.Request(f"{BASE_URL}/api/compile", data=payload, headers=auth_headers)
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  [Pass] POST /api/compile -> success =", data.get("success"))
            assert data.get("success") is True
            assert data.get("hex") is not None

        # Test 4: Oversized Payload -> 413
        large_payload = json.dumps({"code": "x" * (2 * 1024 * 1024), "mcu": "atmega328p"}).encode("utf-8")
        large_req = urllib.request.Request(f"{BASE_URL}/api/compile", data=large_payload, headers=auth_headers)
        try:
            with urllib.request.urlopen(large_req) as resp:
                assert False, "Oversized request should have failed with 413"
        except urllib.error.HTTPError as e:
            print(f"  [Pass] Oversized payload correctly rejected with HTTP {e.code}")
            assert e.code == 413

        # Test 5: Unsupported MCU -> Validation Error
        invalid_mcu_payload = json.dumps({"code": sketch_code, "mcu": "invalid_mcu_999"}).encode("utf-8")
        req = urllib.request.Request(f"{BASE_URL}/api/compile", data=invalid_mcu_payload, headers=auth_headers)
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  [Pass] Unsupported MCU correctly returned validation error ->", data.get("error"))
            assert data.get("success") is False
            assert data.get("stage") == "validation"

        # Test 6: Authorized POST /api/simulate
        req = urllib.request.Request(f"{BASE_URL}/api/simulate", data=payload, headers=auth_headers)
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  [Pass] POST /api/simulate -> success =", data.get("success"))
            assert data.get("success") is True

        print("PASSED: All server security and API smoke tests completed successfully!")

    except Exception as e:
        print("FAILED: Server API smoke test failed with error:", e)
        proc.terminate()
        sys.exit(1)
    finally:
        proc.terminate()
        proc.wait()

if __name__ == "__main__":
    run_tests()
    sys.exit(0)

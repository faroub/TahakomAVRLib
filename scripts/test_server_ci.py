#!/usr/bin/env python3
"""
Server API Smoke Test for TahakomAVR Platform
"""

import sys
import os
import time
import json
import urllib.request
import subprocess

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_APP = os.path.join(PROJECT_ROOT, "server/app.py")
PORT = 8199
BASE_URL = f"http://localhost:{PORT}"

def run_tests():
    print(f"--- Starting local server on port {PORT} for CI smoke testing ---")
    env = os.environ.copy()
    env["PORT"] = str(PORT)
    proc = subprocess.Popen([sys.executable, SERVER_APP], cwd=PROJECT_ROOT, env=env)
    time.sleep(1.5) # Allow server to bind port

    try:
        # Test 1: GET /api/ports
        req = urllib.request.Request(f"{BASE_URL}/api/ports")
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  GET /api/ports ->", data)
            assert "ports" in data

        # Test 2: POST /api/compile
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
        req = urllib.request.Request(f"{BASE_URL}/api/compile", data=payload, headers={"Content-Type": "application/json"})
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  POST /api/compile -> success =", data.get("success"), "stderr:", data.get("stderr"))
            assert data.get("success") is True
            assert data.get("hex") is not None

        # Test 3: POST /api/simulate
        req = urllib.request.Request(f"{BASE_URL}/api/simulate", data=payload, headers={"Content-Type": "application/json"})
        with urllib.request.urlopen(req) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            print("  POST /api/simulate -> success =", data.get("success"), "error:", data.get("error"))
            assert data.get("success") is True

        print("PASSED: Server API smoke tests completed successfully!")

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

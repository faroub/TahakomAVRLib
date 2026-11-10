import os
import json
import asyncio
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import parse_qs, urlparse

from services.compiler import compile_sketch
from services.flasher import flash_hex, list_serial_ports
from services.simulator import simulate_sketch

STATIC_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "static"))

class TahakomServerHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STATIC_DIR, **kwargs)

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path == "/api/ports":
            ports = list_serial_ports()
            self._send_json({"ports": ports})
        else:
            super().do_GET()

    def do_POST(self):
        parsed = urlparse(self.path)
        content_length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(content_length).decode("utf-8") if content_length > 0 else ""

        try:
            data = json.loads(body) if body else {}
        except Exception:
            data = {}

        if parsed.path == "/api/compile":
            code = data.get("code", "")
            mcu = data.get("mcu", "atmega328p")
            res = compile_sketch(code, mcu=mcu)
            self._send_json(res)
        elif parsed.path == "/api/flash":
            hex_content = data.get("hex", "")
            mcu = data.get("mcu", "atmega328p")
            port = data.get("port", None)
            res = flash_hex(hex_content, mcu=mcu, port=port)
            self._send_json(res)
        elif parsed.path == "/api/simulate":
            code = data.get("code", "")
            res = simulate_sketch(code)
            self._send_json(res)
        else:
            self._send_json({"error": "Endpoint not found"}, status=404)

    def _send_json(self, data, status=200):
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(json.dumps(data).encode("utf-8"))

def run_server(port=8000):
    server_address = ("", port)
    httpd = HTTPServer(server_address, TahakomServerHandler)
    print(f"TahakomAVR Dev Environment running at http://localhost:{port}")
    httpd.serve_forever()

if __name__ == "__main__":
    run_server()

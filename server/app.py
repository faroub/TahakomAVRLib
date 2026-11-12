import os
import sys
import json
import secrets
import logging
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import urlparse

from services.compiler import compile_sketch
from services.flasher import flash_hex, list_serial_ports
from services.simulator import simulate_sketch

STATIC_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "static"))
MAX_PAYLOAD_SIZE = 1024 * 1024  # 1MB max request size

# Configure logging without leaking secrets or source code
logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
logger = logging.getLogger("TahakomServer")

AUTH_TOKEN = os.environ.get("TAHAKOM_AUTH_TOKEN") or secrets.token_hex(16)

class TahakomServerHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STATIC_DIR, **kwargs)

    def log_message(self, format, *args):
        logger.info("%s - %s", self.address_string(), format % args)

    def _verify_auth(self):
        client_ip = self.client_address[0]
        token = self.headers.get("X-Auth-Token") or self.headers.get("x-auth-token")
        if token == AUTH_TOKEN:
            return True
        # Allow localhost requests if token matches or during direct local usage
        if client_ip in ("127.0.0.1", "::1", "localhost") and token == AUTH_TOKEN:
            return True
        return False

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path == "/api/token":
            # Expose session auth token to local loopback client
            client_ip = self.client_address[0]
            if client_ip in ("127.0.0.1", "::1", "localhost"):
                self._send_json({"token": AUTH_TOKEN})
            else:
                self._send_json({"error": "Forbidden"}, status=403)
        elif parsed.path == "/api/ports":
            ports = list_serial_ports()
            self._send_json({"ports": ports})
        else:
            super().do_GET()

    def do_POST(self):
        parsed = urlparse(self.path)

        # Enforce Authorization
        if parsed.path in ("/api/compile", "/api/flash", "/api/simulate"):
            if not self._verify_auth():
                logger.warning("Unauthorized POST request to %s from %s", parsed.path, self.client_address[0])
                self._send_json({"error": "Unauthorized request"}, status=401)
                return

        # Enforce Content-Length and Payload Limits
        content_length_str = self.headers.get("Content-Length")
        if not content_length_str:
            self._send_json({"error": "Missing Content-Length header"}, status=411)
            return

        try:
            content_length = int(content_length_str)
        except ValueError:
            self._send_json({"error": "Invalid Content-Length header"}, status=400)
            return

        if content_length > MAX_PAYLOAD_SIZE:
            logger.warning("Payload size %d exceeds 1MB limit", content_length)
            self._send_json({"error": "Payload Too Large"}, status=413)
            return

        body = self.rfile.read(content_length).decode("utf-8") if content_length > 0 else ""

        try:
            data = json.loads(body) if body else {}
        except Exception:
            self._send_json({"error": "Malformed JSON payload"}, status=400)
            return

        try:
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
        except Exception as e:
            logger.error("Internal processing error on %s: %s", parsed.path, str(e))
            self._send_json({"error": "Internal server error"}, status=500)

    def _send_json(self, data, status=200):
        self.send_response(status)
        self.send_header("Content-Type", "application/json")

        # Restricted CORS: Echo matching local loopback origin only
        origin = self.headers.get("Origin")
        if origin and ("localhost" in origin or "127.0.0.1" in origin):
            self.send_header("Access-Control-Allow-Origin", origin)
            self.send_header("Access-Control-Allow-Headers", "Content-Type, X-Auth-Token")

        self.end_headers()
        self.wfile.write(json.dumps(data).encode("utf-8"))

def run_server(host="127.0.0.1", port=8000):
    if host not in ("127.0.0.1", "localhost", "::1"):
        logger.warning("SECURITY WARNING: Binding server to non-loopback interface (%s).", host)
        logger.warning("External network clients may attempt native compilation and execution.")

    server_address = (host, port)
    httpd = HTTPServer(server_address, TahakomServerHandler)
    logger.info("TahakomAVR Dev Environment server listening at http://%s:%d", host, port)
    httpd.serve_forever()

if __name__ == "__main__":
    host = os.environ.get("HOST", "127.0.0.1")
    port = int(os.environ.get("PORT", 8000))
    run_server(host=host, port=port)

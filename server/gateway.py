import os
import json
import secrets
import logging
from http.server import SimpleHTTPRequestHandler
from urllib.parse import urlparse

from services.rate_limiter import rate_limiter
from services.audit_logger import audit_logger
from services.build_service import compile_sketch
from services.hardware_service import flash_hex, list_serial_ports
from services.sandbox_service import simulate_sketch

STATIC_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "static"))
MAX_PAYLOAD_SIZE = 1024 * 1024  # 1MB max request size

logger = logging.getLogger("TahakomGateway")
AUTH_TOKEN = os.environ.get("TAHAKOM_AUTH_TOKEN") or secrets.token_hex(16)

class TahakomGatewayHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STATIC_DIR, **kwargs)

    def _verify_auth(self):
        token = self.headers.get("X-Auth-Token") or self.headers.get("x-auth-token")
        return token == AUTH_TOKEN

    def do_GET(self):
        parsed = urlparse(self.path)
        client_ip = self.client_address[0]

        if parsed.path == "/api/token":
            if client_ip in ("127.0.0.1", "::1", "localhost"):
                audit_logger.log_event(client_ip, parsed.path, "GET_TOKEN", "SUCCESS")
                self._send_json({"token": AUTH_TOKEN})
            else:
                audit_logger.log_event(client_ip, parsed.path, "GET_TOKEN", "FORBIDDEN")
                self._send_json({"error": "Forbidden"}, status=403)
        elif parsed.path == "/api/ports":
            ports = list_serial_ports()
            self._send_json({"ports": ports})
        else:
            super().do_GET()

    def do_POST(self):
        parsed = urlparse(self.path)
        client_ip = self.client_address[0]

        # Stage 1: Rate Limiter Check
        allowed, retry_after = rate_limiter.is_allowed(client_ip)
        if not allowed:
            audit_logger.log_event(client_ip, parsed.path, "RATE_LIMIT", "REJECTED", {"retry_after": retry_after})
            self._send_json({"error": "Too Many Requests", "retry_after": retry_after}, status=429)
            return

        # Stage 2: Authorization Verification
        if parsed.path in ("/api/compile", "/api/flash", "/api/simulate"):
            if not self._verify_auth():
                audit_logger.log_event(client_ip, parsed.path, "AUTH", "UNAUTHORIZED")
                self._send_json({"error": "Unauthorized request"}, status=401)
                return

        # Stage 3: Payload Size & Schema Validation
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
            audit_logger.log_event(client_ip, parsed.path, "VALIDATION", "PAYLOAD_TOO_LARGE", {"size": content_length})
            self._send_json({"error": "Payload Too Large"}, status=413)
            return

        body = self.rfile.read(content_length).decode("utf-8") if content_length > 0 else ""

        try:
            data = json.loads(body) if body else {}
        except Exception:
            self._send_json({"error": "Malformed JSON payload"}, status=400)
            return

        # Stage 4: Microservice Routing
        try:
            if parsed.path == "/api/compile":
                code = data.get("code", "")
                mcu = data.get("mcu", "atmega328p")
                res = compile_sketch(code, mcu=mcu)
                audit_logger.log_event(client_ip, parsed.path, "BUILD_SERVICE", "SUCCESS" if res.get("success") else "FAILED", {"mcu": mcu})
                self._send_json(res)

            elif parsed.path == "/api/flash":
                hex_content = data.get("hex", "")
                mcu = data.get("mcu", "atmega328p")
                port = data.get("port", None)
                res = flash_hex(hex_content, mcu=mcu, port=port)
                audit_logger.log_event(client_ip, parsed.path, "HARDWARE_SERVICE", "SUCCESS" if res.get("success") else "FAILED", {"mcu": mcu, "port": port})
                self._send_json(res)

            elif parsed.path == "/api/simulate":
                code = data.get("code", "")
                res = simulate_sketch(code)
                audit_logger.log_event(client_ip, parsed.path, "SANDBOX_SERVICE", "SUCCESS" if res.get("success") else "FAILED")
                self._send_json(res)

            else:
                self._send_json({"error": "Endpoint not found"}, status=404)

        except Exception as e:
            logger.error("Internal processing error on %s: %s", parsed.path, str(e))
            audit_logger.log_event(client_ip, parsed.path, "SERVICE_ERROR", "ERROR", {"error": str(e)})
            self._send_json({"error": "Internal server error"}, status=500)

    def _send_json(self, data, status=200):
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        origin = self.headers.get("Origin")
        if origin and ("localhost" in origin or "127.0.0.1" in origin):
            self.send_header("Access-Control-Allow-Origin", origin)
            self.send_header("Access-Control-Allow-Headers", "Content-Type, X-Auth-Token")
        self.end_headers()
        self.wfile.write(json.dumps(data).encode("utf-8"))

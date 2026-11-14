import os
import json
import hmac
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

def validate_host_header(host_str: str, server_port: int = 8000) -> bool:
    """Strictly validate Host header hostname and port."""
    if not host_str or not isinstance(host_str, str):
        return False

    host_str = host_str.strip().lower()

    if host_str.startswith("["):
        parts = host_str.split("]")
        hostname = parts[0] + "]"
        port_part = parts[1].lstrip(":") if len(parts) > 1 else ""
    else:
        parts = host_str.split(":")
        hostname = parts[0]
        port_part = parts[1] if len(parts) > 1 else ""

    if hostname not in ("127.0.0.1", "localhost", "::1", "[::1]"):
        return False

    if port_part:
        try:
            port_num = int(port_part)
            if port_num != server_port:
                return False
        except ValueError:
            return False

    return True

def validate_origin(origin_str: str, server_port: int = 8000) -> bool:
    """Strictly parse and validate Origin scheme, hostname, and port."""
    if not origin_str or not isinstance(origin_str, str):
        return False
    try:
        parsed = urlparse(origin_str)
    except Exception:
        return False

    if parsed.username or parsed.password:
        return False

    if parsed.scheme not in ("http", "https"):
        return False

    hostname = (parsed.hostname or "").lower()
    if hostname not in ("127.0.0.1", "localhost", "::1", "[::1]"):
        return False

    port = parsed.port
    if port is not None:
        if port not in (8000, 8199, 8299, 8399, server_port):
            return False

    return True

class TahakomGatewayHandler(SimpleHTTPRequestHandler):
    server_port = 8000
    timeout = 5.0  # Per-connection socket read timeout

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STATIC_DIR, **kwargs)

    def _verify_auth(self):
        token = self.headers.get("X-Auth-Token") or self.headers.get("x-auth-token")
        if not token or not isinstance(token, str):
            return False
        return hmac.compare_digest(token, AUTH_TOKEN)

    def _validate_request_headers(self) -> bool:
        """Validate Host and Origin headers for all HTTP methods."""
        host_header = self.headers.get("Host")
        if not validate_host_header(host_header, self.server_port):
            self._send_json({"error": "Forbidden Host header"}, status=403)
            return False

        origin_header = self.headers.get("Origin")
        if origin_header and not validate_origin(origin_header, self.server_port):
            self._send_json({"error": "Forbidden Origin header"}, status=403)
            return False

        return True

    def do_OPTIONS(self):
        if not self._validate_request_headers():
            return
        origin = self.headers.get("Origin")
        self.send_response(204)
        if origin and validate_origin(origin, self.server_port):
            self.send_header("Access-Control-Allow-Origin", origin)
            self.send_header("Access-Control-Allow-Headers", "Content-Type, X-Auth-Token")
            self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.end_headers()

    def do_PUT(self):
        if not self._validate_request_headers():
            return
        self._send_json({"error": "Method Not Allowed"}, status=405)

    def do_DELETE(self):
        if not self._validate_request_headers():
            return
        self._send_json({"error": "Method Not Allowed"}, status=405)

    def do_PATCH(self):
        if not self._validate_request_headers():
            return
        self._send_json({"error": "Method Not Allowed"}, status=405)

    def do_GET(self):
        if not self._validate_request_headers():
            return

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
            if not self._verify_auth():
                audit_logger.log_event(client_ip, parsed.path, "AUTH", "UNAUTHORIZED")
                self._send_json({"error": "Unauthorized request"}, status=401)
                return
            ports = list_serial_ports()
            self._send_json({"ports": ports})

        elif parsed.path.startswith("/api/"):
            self._send_json({"error": "Endpoint not found"}, status=404)

        else:
            super().do_GET()

    def do_POST(self):
        if not self._validate_request_headers():
            return

        parsed = urlparse(self.path)
        client_ip = self.client_address[0]

        if self.headers.get("Transfer-Encoding"):
            self._send_json({"error": "Unsupported Transfer-Encoding"}, status=400)
            return

        allowed, retry_after = rate_limiter.is_allowed(client_ip)
        if not allowed:
            audit_logger.log_event(client_ip, parsed.path, "RATE_LIMIT", "REJECTED", {"retry_after": retry_after})
            self._send_json({"error": "Too Many Requests", "retry_after": retry_after}, status=429)
            return

        if parsed.path in ("/api/compile", "/api/flash", "/api/simulate"):
            if not self._verify_auth():
                audit_logger.log_event(client_ip, parsed.path, "AUTH", "UNAUTHORIZED")
                self._send_json({"error": "Unauthorized request"}, status=401)
                return

        content_length_str = self.headers.get("Content-Length")
        if not content_length_str:
            self._send_json({"error": "Missing Content-Length header"}, status=411)
            return

        try:
            content_length = int(content_length_str)
        except ValueError:
            self._send_json({"error": "Invalid Content-Length header"}, status=400)
            return

        if content_length < 0:
            self._send_json({"error": "Content-Length cannot be negative"}, status=400)
            return

        if content_length > MAX_PAYLOAD_SIZE:
            audit_logger.log_event(client_ip, parsed.path, "VALIDATION", "PAYLOAD_TOO_LARGE", {"size": content_length})
            self._send_json({"error": "Payload Too Large"}, status=413)
            return

        content_type = self.headers.get("Content-Type", "")
        if "application/json" not in content_type.lower():
            self._send_json({"error": "Content-Type must be application/json"}, status=415)
            return

        body = self.rfile.read(content_length).decode("utf-8") if content_length > 0 else ""

        try:
            data = json.loads(body) if body else {}
        except Exception:
            self._send_json({"error": "Malformed JSON payload"}, status=400)
            return

        if not isinstance(data, dict):
            self._send_json({"error": "Top-level JSON payload must be a JSON object"}, status=400)
            return

        try:
            if parsed.path == "/api/compile":
                code = data.get("code", "")
                mcu = data.get("mcu", "atmega328p")
                f_cpu = data.get("f_cpu", 16000000)
                baud = data.get("baud", 9600)

                if not isinstance(code, str) or not isinstance(mcu, str):
                    self._send_json({"error": "Invalid payload field types"}, status=400)
                    return

                if isinstance(f_cpu, bool) or not isinstance(f_cpu, int) or isinstance(baud, bool) or not isinstance(baud, int):
                    self._send_json({"error": "f_cpu and baud must be non-boolean integers"}, status=400)
                    return

                res = compile_sketch(code, mcu=mcu, f_cpu=f_cpu, baud=baud)
                audit_logger.log_event(client_ip, parsed.path, "BUILD_SERVICE", "SUCCESS" if res.get("success") else "FAILED", {"mcu": mcu})
                self._send_json(res)

            elif parsed.path == "/api/flash":
                hex_content = data.get("hex", "")
                mcu = data.get("mcu", "atmega328p")
                programmer = data.get("programmer", "arduino")
                port = data.get("port", None)
                confirmed = data.get("confirmed")
                baud = data.get("baud", 115200)

                if not isinstance(hex_content, str) or not isinstance(mcu, str) or not isinstance(programmer, str) or (port is not None and not isinstance(port, str)):
                    self._send_json({"error": "Invalid payload field types"}, status=400)
                    return

                if isinstance(baud, bool) or not isinstance(baud, int):
                    self._send_json({"error": "baud must be integer"}, status=400)
                    return

                # Strict literal boolean True requirement (S5)
                if confirmed is not True:
                    self._send_json({"error": "Explicit user confirmation (confirmed: true) is required"}, status=400)
                    return

                res = flash_hex(hex_content, mcu=mcu, programmer=programmer, port=port, baud=baud, confirmed=True)
                audit_logger.log_event(client_ip, parsed.path, "HARDWARE_SERVICE", "SUCCESS" if res.get("success") else "FAILED", {"mcu": mcu, "port": port})
                self._send_json(res)

            elif parsed.path == "/api/simulate":
                code = data.get("code", "")
                if not isinstance(code, str):
                    self._send_json({"error": "Invalid payload field types"}, status=400)
                    return
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
        if origin and validate_origin(origin, self.server_port):
            self.send_header("Access-Control-Allow-Origin", origin)
            self.send_header("Access-Control-Allow-Headers", "Content-Type, X-Auth-Token")
            self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")

        self.end_headers()
        self.wfile.write(json.dumps(data).encode("utf-8"))

import os
import logging
from socketserver import ThreadingMixIn
from http.server import HTTPServer
from gateway import TahakomGatewayHandler

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
logger = logging.getLogger("TahakomServer")

class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    daemon_threads = True
    timeout = 5.0

def run_server(host="127.0.0.1", port=8000):
    if host not in ("127.0.0.1", "localhost", "::1"):
        logger.warning("SECURITY WARNING: Binding server to non-loopback interface (%s).", host)
        logger.warning("External network clients may attempt native compilation and execution.")

    TahakomGatewayHandler.server_port = port
    server_address = (host, port)
    httpd = ThreadedHTTPServer(server_address, TahakomGatewayHandler)
    logger.info("TahakomAVR Dev Environment server listening at http://%s:%d", host, port)
    httpd.serve_forever()

if __name__ == "__main__":
    host = os.environ.get("HOST", "127.0.0.1")
    port = int(os.environ.get("PORT", 8000))
    run_server(host=host, port=port)

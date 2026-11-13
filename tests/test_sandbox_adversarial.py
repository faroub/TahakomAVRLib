import unittest
import os
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SERVER_DIR = os.path.join(PROJECT_ROOT, "server")
sys.path.insert(0, SERVER_DIR)

from services.sandbox_service import simulate_sketch, is_sandbox_available

class TestSandboxAdversarial(unittest.TestCase):

    def setUp(self):
        if not is_sandbox_available():
            self.skipTest("Bubblewrap (bwrap) sandbox is not available on this host environment")

    def test_normal_sketch_passes(self):
        code = """
#include "host_registers.h"
#include <stdio.h>

int main(void) {
    printf("NORMAL_SIMULATION_OK\\n");
    return 0;
}
        """
        res = simulate_sketch(code)
        self.assertTrue(res.get("success"), f"Normal sketch failed: {res}")
        self.assertIn("NORMAL_SIMULATION_OK", res.get("stdout", ""))

    def test_infinite_loop_timeout_fails(self):
        code = """
#include "host_registers.h"

int main(void) {
    while(1) {}
    return 0;
}
        """
        res = simulate_sketch(code)
        self.assertFalse(res.get("success"), "Infinite loop must NOT return success=True")
        self.assertIn("timeout", res.get("error", "").lower())

    def test_filesystem_read_blocked(self):
        code = """
#include "host_registers.h"
#include <stdio.h>

int main(void) {
    FILE *f = fopen("/etc/passwd", "r");
    if (f) {
        printf("READ_SUCCESS\\n");
        fclose(f);
    } else {
        printf("READ_BLOCKED\\n");
    }
    return 0;
}
        """
        res = simulate_sketch(code)
        self.assertTrue(res.get("success"))
        self.assertIn("READ_BLOCKED", res.get("stdout", ""))
        self.assertNotIn("READ_SUCCESS", res.get("stdout", ""))

    def test_filesystem_write_blocked(self):
        code = """
#include "host_registers.h"
#include <stdio.h>

int main(void) {
    FILE *f = fopen("/etc/evil.txt", "w");
    if (f) {
        printf("WRITE_SUCCESS\\n");
        fclose(f);
    } else {
        printf("WRITE_BLOCKED\\n");
    }
    return 0;
}
        """
        res = simulate_sketch(code)
        self.assertTrue(res.get("success"))
        self.assertIn("WRITE_BLOCKED", res.get("stdout", ""))
        self.assertNotIn("WRITE_SUCCESS", res.get("stdout", ""))

    def test_huge_stdout_truncated_or_failed(self):
        code = """
#include "host_registers.h"
#include <stdio.h>

int main(void) {
    for (int i = 0; i < 50000; i++) {
        printf("AAAAA_EXTRA_LONG_LINE_OUTPUT_FLOOD_TESTING_DATA_0123456789\\n");
    }
    return 0;
}
        """
        res = simulate_sketch(code)
        # Should either truncate output or fail with output limit exceeded
        if res.get("success"):
            self.assertLessEqual(len(res.get("stdout", "")), 16384 + 1024)
        else:
            self.assertIn("output limit", res.get("error", "").lower())

    def test_network_access_blocked(self):
        code = """
#include "host_registers.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("SOCKET_BLOCKED\\n");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = 0x0100007f; // 127.0.0.1
    int res = connect(s, (struct sockaddr*)&addr, sizeof(addr));
    if (res < 0) {
        printf("CONNECT_BLOCKED\\n");
    } else {
        printf("CONNECT_SUCCESS\\n");
    }
    return 0;
}
        """
        res = simulate_sketch(code)
        self.assertTrue(res.get("success"))
        self.assertIn("CONNECT_BLOCKED", res.get("stdout", ""))

if __name__ == "__main__":
    unittest.main()

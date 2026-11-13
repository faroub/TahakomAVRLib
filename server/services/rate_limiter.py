import time
import threading
from collections import OrderedDict

class SlidingWindowRateLimiter:
    def __init__(self, max_requests: int = 15, window_seconds: int = 10, max_tracked_ips: int = 10000):
        self.max_requests = max_requests
        self.window_seconds = window_seconds
        self.max_tracked_ips = max_tracked_ips
        self.requests = OrderedDict()
        self.lock = threading.Lock()

    def is_allowed(self, client_ip: str) -> tuple[bool, int]:
        with self.lock:
            now = time.monotonic()
            cutoff = now - self.window_seconds

            # LRU pruning if map size exceeds max capacity
            if len(self.requests) >= self.max_tracked_ips and client_ip not in self.requests:
                self.requests.popitem(last=False)

            timestamps = self.requests.get(client_ip, [])
            timestamps = [t for t in timestamps if t > cutoff]

            if len(timestamps) >= self.max_requests:
                oldest = timestamps[0]
                retry_after = int(self.window_seconds - (now - oldest)) + 1
                self.requests[client_ip] = timestamps
                self.requests.move_to_end(client_ip)
                return False, max(1, retry_after)

            timestamps.append(now)
            self.requests[client_ip] = timestamps
            self.requests.move_to_end(client_ip)
            return True, 0

# Global instance
rate_limiter = SlidingWindowRateLimiter(max_requests=15, window_seconds=10, max_tracked_ips=10000)

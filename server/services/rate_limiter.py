import time
from collections import defaultdict

class SlidingWindowRateLimiter:
    def __init__(self, max_requests: int = 15, window_seconds: int = 10):
        self.max_requests = max_requests
        self.window_seconds = window_seconds
        self.requests = defaultdict(list)

    def is_allowed(self, client_ip: str) -> tuple[bool, int]:
        now = time.time()
        cutoff = now - self.window_seconds

        # Clean expired timestamps
        timestamps = [t for t in self.requests[client_ip] if t > cutoff]
        self.requests[client_ip] = timestamps

        if len(timestamps) >= self.max_requests:
            oldest = timestamps[0]
            retry_after = int(self.window_seconds - (now - oldest)) + 1
            return False, max(1, retry_after)

        self.requests[client_ip].append(now)
        return True, 0

# Global instance
rate_limiter = SlidingWindowRateLimiter(max_requests=15, window_seconds=10)

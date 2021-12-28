import threading
import time

import spam


class Ticker:
    def __init__(self, delay):
        self.cnt = 0
        self.delay = delay

    def tick(self):
        while True:
            self.cnt += 1
            print("Tick %d..." % self.cnt)
            time.sleep(self.delay)


if __name__ == "__main__":
    threading.Thread(target=Ticker(1).tick, args=(), daemon=True).start()
    spam.print_list(["test1 test2"])

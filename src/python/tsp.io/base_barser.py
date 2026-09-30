from abc import ABC, abstractmethod
import gzip

class BaseParser(ABC):
    def __init__(self, filepath: str):
        self.filepath = filepath
        self.dimension = None

    def read_lines(self):
    # Shared utility: open both .gz and plain text transparently
        if self.filepath.endswith(".gz"):
            with gzip.open(self.filepath, mode="rt", encoding="utf-8") as f:
                yield from (line.strip() for line in f if line.strip())
        else:
            with open(self.filepath, mode="r", encoding="utf-8") as f:
                yield from (line.strip() for line in f if line.strip())

    @abstractmethod
    def parse(self):
        """Every subclass must implement its own parse logic."""
        pass
import numpy as np
from .base_parser import BaseParser

class LowerTriParser(BaseParser):
    def parse(self):
        matrix_data = []
        reading_matrix = False

        for line in self.read_lines():
            if line.startswith("DIMENSION"):
                self.dimension = int(line.split(":")[-1].strip())
            elif line.startswith("EDGE_WEIGHT_SECTION"):
                reading_matrix = True
            elif reading_matrix:
                if line == "EOF":
                    break
                
                # Safely convert tokens, skipping non-numeric entries if any exist
                for val in line.split():
                    try:
                        matrix_data.append(float(val))
                    except ValueError:
                        continue

        return self.dimension, np.array(matrix_data)
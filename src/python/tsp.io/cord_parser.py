import numpy as np
import gzip
from .base_parser import BaseParser

# extract: DIMENSION (line 4), 
# then calc matrix for every node i following NODE_COORD_SECTION (starting line 7)
# NODE_COORD_SECTION
# 1 565.0 575.0
# 2 25.0 185.0
# 3 345.0 750.0
# idx float float


#...
# EOF (line idx_n +1)


# then we compute pairwise Euclidean distance
# matrix as 2d np array

# then we convert matrix to lower triangle 1d arrayX
class CoordParser(BaseParser):
    def parse(self):
        dim = None
        matrix = np.zeros((dim, dim))  # Initialize empty matrix of size dim x dim

        reading_coords = False

        # Open the .gz file in text mode (mode = 'rt')
        with gzip.open(self.filepath, mode="rt", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue

                # 1. Extract DIMENSION
                if line.startswith("DIMENSION"):
                    dimension = int(line.split(":")[-1].strip())
                    continue

                # 2. Check for start of coordinates section
                if line.startswith("NODE_COORD_SECTION"):
                    reading_coords = True
                    continue

                # 3. Stop if we reach the end of the section or file
                if reading_coords:
                    if line in ("EOF"):
                        break

                    # Parse cords: node_id, x, y
                    parts = line.split()
                    if len(parts) >= 3:
                        node_id = int(parts[0])
                        x = float(parts[1])
                        y = float(parts[2])
                        matrix[node_id - 1] = (x, y)



        return dim, matrix


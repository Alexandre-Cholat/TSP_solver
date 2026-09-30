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

# matrix as 2d np array

class CoordParser(BaseParser):
    
    def parse(self):
        dim = 0
        matrix = None
        reading_coords = False

        # Open the .gz file in text mode (mode = 'rt')
        with gzip.open(self.filepath, mode="rt", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue

                # 1. Extract DIMENSION
                if line.startswith("DIMENSION"):
                    dim = int(line.split(":")[-1].strip())
                    matrix = np.zeros((dim, 2))  # Initialize empty matrix of size dim x 2 for coordinates
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
                        matrix[node_id - 1][0] = x
                        matrix[node_id - 1][1] = y



        return dim, matrix


    # then we compute pairwise Euclidean distance

    # then we convert matrix to lower triangle 1d arrayX
    def compute_distance_matrix(self, cords_matrix):
        dim = len(cords_matrix)
        matrix = np.zeros((dim, dim))  # init empty np array

        # for i in dim
            # calulate pairwise Euclidean distance from i to all others (strictly greater idx than i), fill in matrix
        for i in range(dim):
            for j in range(i, dim):
                dist = np.linalg.norm(cords_matrix[i] - cords_matrix[j])
                matrix[i][j] = dist
                matrix[j][i] = dist  # symmetric

        # flatten matrix to lower triangle 1d arrayX and return it
        lower_triangle = matrix[np.tril_indices(dim, k=-1)]
        return lower_triangle

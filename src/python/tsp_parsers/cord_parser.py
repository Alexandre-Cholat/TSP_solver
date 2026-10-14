import numpy as np
from .base_parser import BaseParser

class CoordParser(BaseParser):
    
    def parse(self):
        reading_coords = False
        coords_list = []

        # 1. Read through lines using BaseParser's read_lines utility
        for line in self.read_lines():
            if line.startswith("DIMENSION"):
                self.dimension = int(line.split(":")[-1].strip())
                continue

            if line.startswith("NODE_COORD_SECTION"):
                reading_coords = True
                continue

            if reading_coords:
                if line == "EOF":
                    break

                parts = line.split()
                if len(parts) >= 3:
                    node_id = int(parts[0]) - 1  # 1-indexed to 0-indexed
                    x = float(parts[1])
                    y = float(parts[2])
                    coords_list.append((node_id, x, y))


        # Sort by node_id to ensure proper ordering and store matrix
        coords_list.sort(key=lambda item: item[0])
        coords_matrix = np.array([[x, y] for _, x, y in coords_list])

        # 3. Compute lower triangular distance array
        lower_tri_weights = self.compute_distance_matrix(coords_matrix)

        # Return standardized format: (dimension, lower_triangular_weights_1d)
        return self.dimension, lower_tri_weights

    def compute_distance_matrix(self, coords_matrix: np.ndarray) -> np.ndarray:
        """
        Calculates pairwise Euclidean distances rounded to integer (TSPLIB standard) 
        and flattens the matrix into 1D lower-triangular format (LOWER_DIAG_ROW).
        """
        dim = len(coords_matrix)
        dist_matrix = np.zeros((dim, dim))

        for i in range(dim):
            for j in range(i + 1, dim):
                d = np.linalg.norm(coords_matrix[i] - coords_matrix[j])
                dist = round(d)  # Standard TSPLIB rounding for Euclidean distance
                dist_matrix[i][j] = dist
                dist_matrix[j][i] = dist

        # Extract lower triangular entries (k=-1 excludes diagonal)
        tril_indices = np.tril_indices(dim, k=-1)

        
        return dist_matrix[tril_indices]
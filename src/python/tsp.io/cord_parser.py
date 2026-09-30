# activate uv with: source .venv/bin/activate
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

# then we convert matrix to lower triangle 1d array

import numpy as np

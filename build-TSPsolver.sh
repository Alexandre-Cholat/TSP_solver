#!/bin/bash

# Exit immediately if any command fails
set -e
cd build
cmake ..
make

echo "Build complete. Run the TSP solver with :"
echo "  ./TSPsolver <instance_path> <output_path> [-m <algorithm>]"
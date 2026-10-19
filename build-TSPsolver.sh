#!/bin/bash

set -e
# create build dir if not already exists
mkdir -p build
cd build
cmake ..
make

echo "Build complete. Run the TSP solver with :"
echo "  ./TSPsolver <instance_path> <output_path> [-m <algorithm>]"
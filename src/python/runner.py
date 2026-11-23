import sys
import json
from pathlib import Path
from tsp_parsers import CoordParser, LowerTriParser
import numpy as np
import sys
import os
import argparse


import tsp_bindings

# Assuming your parser classes are saved in a file named `parsers.py`
from tsp_parsers import CoordParser, LowerTriParser 

def get_parser(file_path):
    """Scans the file to determine the correct parser based on keywords."""
    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith("NODE_COORD_SECTION"):
                return CoordParser(file_path)
            elif line.startswith("EDGE_WEIGHT_SECTION"):
                return LowerTriParser(file_path)
    raise ValueError(f"Could not determine format for {file_path}")

def main():
    # 1. Handle command-line arguments
    parser = argparse.ArgumentParser(description="TSP Solver")
    parser.add_argument("instance_path", help="Path to the instance file")
    parser.add_argument("output_path", help="Path to the output file")
    parser.add_argument("-m", "--method", default="trivial", help="Solving algorithm")
    
    args = parser.parse_args()

    algo_map = {
        "trivial": 1,
        "held-karp": 2
    }
    
    algo_type = algo_map.get(args.method.lower())
    if algo_type is None:
        print(f"Error: Unknown method '{args.method}'.")
        sys.exit(1)

    # select the correct parser and parse
    tsp_parser = get_parser(args.instance_path)
    dimension, weights = tsp_parser.parse()

    # build a single 1D numpy array
    # type float64 to match py::array_t<double> in C++
    header = np.array([dimension, algo_type], dtype=np.float64)
    parsed_array = np.concatenate((header, weights)).astype(np.float64)

    # call the C++ solver
    try:
        # C++ returns a list of integers representing the node permutation
        tour = tsp_bindings.solve_tsp(parsed_array)
    except Exception as e:
        print(f"C++ solver execution failed: {e}")
        sys.exit(1)

    # write output file
    instance_name = os.path.basename(args.instance_path).split('.')[0]
    
    with open(args.output_path, 'w') as out_f:
        out_f.write(f"NAME: {instance_name}.tour\n")
        out_f.write("TYPE: TOUR\n")
        out_f.write(f"DIMENSION: {dimension}\n")
        out_f.write("TOUR_SECTION\n")
        
        for node in tour:
            out_f.write(f"{node}\n")
            
        out_f.write("-1\n")
        out_f.write("EOF\n")

if __name__ == "__main__":
    main()
import sys
import json
from pathlib import Path
from tsp_parsers import CoordParser, LowerTriParser
import numpy as np
import fast_engine

#TODO : add function to insert dim and desired algo type as first two elements of the array to be passed to cpp engine.

def main():
    # build orchestrator here... can be broken into sub functions
    # first decide which parser to use

    # then parse. for testing : 
    parsed_array = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float64)

 
    # cpp calculates and returns an array of size dim +1
    output_array = fast_engine.solve_tsp(parsed_array)



    # print output in proper format:
    output_array = np.array([ 1, 3, 5, 2, 4])   
    # from [ 1, 3, 5, 2, 4] to:

    # NAME : example.tour
    # TYPE : TOUR
    # DIMENSION : 5
    # 1
    # 3
    # 5
    # 2
    # 4
    # -1
    # EOF


    return 1


def resolve_instance_path(filename: str) -> Path:
    path_obj = Path(filename)
    if path_obj.exists():
        return path_obj.resolve()
    
    project_root = Path(__file__).resolve().parents[2]
    root_file = project_root / filename
    if root_file.exists():
        return root_file.resolve()

    raise FileNotFoundError(f"Could not find '{filename}'.")

def parse_instance(filepath: Path):
    str_path = str(filepath)
    try:
        coord_parser = CoordParser(str_path)
        dim, weights = coord_parser.parse()
        if len(weights) > 0:
            print("[Python Parser] Parsed coordinates successfully.")
            return dim, weights
    except Exception as e:
        print(f"[Python Parser] CoordParser skipped: {e}")

    tri_parser = LowerTriParser(str_path)
    dim, weights = tri_parser.parse()
    print("[Python Parser] Parsed matrix successfully.")
    return dim, weights

def main():
    raw_input = sys.argv[1] if len(sys.argv) > 1 else "ali535.tsp.gz"
    target_path = resolve_instance_path(raw_input)

    print(f"--- Parsing Instance: {target_path.name} ---")
    dim, weights = parse_instance(target_path)

    # Export parsed data to a simple JSON format for C++ to consume
    parsed_data = {
        "dimension": dim,
        "weights": weights.tolist() if hasattr(weights, "tolist") else list(weights)
    }

    export_path = target_path.with_suffix(".parsed.json")
    with open(export_path, "w") as f:
        json.dump(parsed_data, f)

    print(f"Extracted Dimension: {dim}")
    print(f"Extracted Total Weights: {len(weights)}")
    print(f"Exported parsed data to: {export_path}")

if __name__ == "__main__":
    main()
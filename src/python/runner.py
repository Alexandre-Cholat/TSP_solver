# activate uv with: source .venv/bin/activate
# download requirements: uv pip install -r requirements.txt
# add requirements : uv pip freeze > requirements.txt

from tsp_parsers import CoordParser, LowerTriParser


def main():
    p = CoordParser("/home/alexandre/Downloads/berlin52.tsp.gz")
    dim, cords_matrix = p.parse()
    print(f"Extracted Dimension: {dim}")
    print(f"Total Nodes Parsed: {len(cords_matrix)}")
    
    # Display the first 5 parsed nodes
    for i in range(5):
        print(f"Node {i}: {cords_matrix[i]}")

    # Compute the distance matrix
    lower_triangle = p.compute_distance_matrix(cords_matrix)
    print(f"Lower Triangle: {lower_triangle}")
    print(f"Lower Triangle Length: {len(lower_triangle)}")
    print(f"Expected Length: {dim * (dim - 1) // 2}")

if __name__ == "__main__":
    main()
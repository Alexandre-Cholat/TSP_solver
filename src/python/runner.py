# activate uv with: source .venv/bin/activate
# download requirements: uv pip install -r requirements.txt
# add requirements : uv pip freeze > requirements.txt

from tsp_parsers import CoordParser, LowerTriParser


def main():
    # Once you reference the classes below, they will not be grayed out
    p = CoordParser("/home/alexandre/Downloads/berlin52.tsp.gz")
    dim, cords_matrix = p.parse()
    print(f"Extracted Dimension: {dim}")
    print(f"Total Nodes Parsed: {len(cords_matrix)}")
    
    # Display the first 5 parsed nodes
    for i in range(5):
        print(f"Node {i}: {cords_matrix[i]}")
if __name__ == "__main__":
    main()
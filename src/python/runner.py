# activate uv with: source .venv/bin/activate
# download requirements: uv pip install -r requirements.txt
# add requirements : uv pip freeze > requirements.txt

from tsp_parsers import CoordParser, LowerTriParser

if __name__ == "__main__":
    file_path = "berlin52.tsp.gz"  # or "berlin52.opt.tour.gz"
    
    dim, nodes = parse_cords(file_path)
    
    print(f"Extracted Dimension: {dim}")
    print(f"Total Nodes Parsed: {len(nodes)}")
    
    # Display the first 5 parsed nodes
    for node_id in list(nodes.keys())[:5]:
        print(f"Node {node_id}: {nodes[node_id]}")


def main():
    # Once you reference the classes below, they will not be grayed out
    p = CoordParser("berlin52.tsp.gz")
    dim, nodes = p.parse()
    print(f"Extracted Dimension: {dim}")
    print(f"Total Nodes Parsed: {len(nodes)}")
    
    # Display the first 5 parsed nodes
    for node_id in list(nodes.keys())[:5]:
        print(f"Node {node_id}: {nodes[node_id]}")
if __name__ == "__main__":
    main()
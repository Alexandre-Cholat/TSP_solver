# activate uv with: source .venv/bin/activate
# download requirements: uv pip install -r requirements.txt
# add requirements : uv pip freeze > requirements.txt

from tsp.io.cord_parser import cord_parser

if __name__ == "__main__":
    file_path = "berlin52.tsp.gz"  # or "berlin52.opt.tour.gz"
    
    dim, nodes = parse_cords(file_path)
    
    print(f"Extracted Dimension: {dim}")
    print(f"Total Nodes Parsed: {len(nodes)}")
    
    # Display the first 5 parsed nodes
    for node_id in list(nodes.keys())[:5]:
        print(f"Node {node_id}: {nodes[node_id]}")
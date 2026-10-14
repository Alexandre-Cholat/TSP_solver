from tsp_parsers import CoordParser, LowerTriParser

def main():
    # Test 1: Coordinate file parsing
    coord_p = CoordParser("ali535.tsp.gz")
    dim1, weights1 = coord_p.parse()
    
    print("--- CoordParser Output ---")
    print(f"Dimension: {dim1}")
    print(f"Total Weights in Lower Triangle: {len(weights1)}")
    print(f"First 5 Distances: {weights1[:5]}\n")

    # Test 2: Lower Triangle matrix file parsing (if you have one)
    tri_p = LowerTriParser("pa561.tsp.gz")
    dim2, weights2 = tri_p.parse()

    print("--- LowerTriParser Output ---")
    print(f"Dimension: {dim2}")
    print(f"Total Weights in Lower Triangle: {len(weights2)}")
    print(f"First 5 Distances: {weights2[:5]}\n")



if __name__ == "__main__":
    main()
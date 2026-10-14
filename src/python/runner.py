from tsp_parsers import CoordParser, LowerTriParser
import numpy as np

def main():
    # build orchestrator here... can be broken into sub functions
    # first decide which parser to use

    # then parse

    # pass pares array to cpp : returns an array of size dim +1

    # print output in proper format
    
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



if __name__ == "__main__":
    main()
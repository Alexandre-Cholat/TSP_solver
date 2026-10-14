from tsp_parsers import CoordParser, LowerTriParser
import numpy as np
import fast_engine

def main():
    # build orchestrator here... can be broken into sub functions
    # first decide which parser to use

    # then parse. for testing : 
    parsed_array = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float64)

    # pass parsed array to cpp
    fast_engine.process_inplace(parsed_array)
    # calculates and returns an array of size dim +1
    processed = fast_engine.process_and_return(parsed_array)



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



if __name__ == "__main__":
    main()
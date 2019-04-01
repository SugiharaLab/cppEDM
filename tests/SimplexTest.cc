// Test simplex algorithm

#include "TestCommon.h"

int main () {

    // Declare DataFrames to hold file and computed output
    DataFrame< double > pyOutput;
    DataFrame< double > cppOutput;

    //----------------------------------------------------------
    // Simplex prediction on block_3sp data : No dynamic embedding
    // Load pyEDM output from:
    //   ./Predict.py -i block_3sp.csv -r x_t -c x_t y_t z_t
    //   -e -l 1 99 -p 100 198 -T 1 -P -o Smplx_embd_block_3sp_pyEDM.csv
    //----------------------------------------------------------
    pyOutput = DataFrame < double >("./data/","Smplx_embd_block_3sp_pyEDM.csv");
    
    // Generate cpp output
    cppOutput = Simplex ( "../data/", "block_3sp.csv",
                          "./data/", "Smplx_embd_block_3sp_cppEDM.csv",
                          "1 99","100 198", 3, 1, 0, 1,
                          "x_t y_t z_t", "x_t", true, true );
    // Comparison
    MakeTest ( "block_3sp.csv embedded data test", pyOutput, cppOutput );

    //----------------------------------------------------------
    // Simplex prediction with dynamically embedded data
    // Load pyEDM output from:
    //   ./Predict.py -i block_3sp.csv -r x_t -c x_t -E 3
    //   -l 1 100 -p 101 195 -T 1 -P -o Smplx_E3_block_3sp_pyEDM.csv
    //----------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_E3_block_3sp_pyEDM.csv");
    
    // Generate cpp output
    cppOutput = Simplex ( "../data/", "block_3sp.csv",
                          "./data/", "Smplx_E3_block_3sp_cppEDM.csv",
                          "1 100", "101 195", 3, 1, 0, 1,
                          "x_t", "x_t", false, true );
    // Comparison
    MakeTest ( "block_3sp.csv dynamic embedding test", pyOutput, cppOutput );
}

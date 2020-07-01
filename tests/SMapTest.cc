// SMap test

#include "TestCommon.h"

int main () {

    //---------------------------------------------------------
    // test smap with simple circle test
    //  - already embedded 
    //---------------------------------------------------------
    // Load pyEdm output from:
    //   ./Predict.py -i circle.csv -r x -c x y -e -m smap -t 4
    //   -l 1 100 -p 101 198 -T 1 -P -o Smap_circle_pyEDM.csv
    //---------------------------------------------------------
    DataFrame < double > coefOutput( "./data/", "Smap_circle_coef_valid.csv" );
    
    // Generate cpp output
    DataFrame < double > circleData( "../data/", "circle.csv" );
    
    SMapValues smapVals = SMap ( circleData,
                                 "./data/", "",
                                 " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "Smap_circle_coef.csv", "",
                                 true, false, false );
    
    DataFrame < double > cppOutput = smapVals.predictions;
    DataFrame < double > cppCoef   = smapVals.coefficients;
    
    // Comparison
    MakeTest ( "SMap: circle test", coefOutput, cppCoef );


    //---------------------------------------------------------
    // Load pyEdm output from:
    //   ./Predict.py -i block_3sp.csv -r x_t -c x_t y_t z_t -e -m smap -t 2
    //   -l 1 99 -p 100 198 -T 1 -P -o Smap_embd_block_3sp_pyEDM.csv
    //---------------------------------------------------------
    DataFrame < double > pyOutput =
        DataFrame <double> ("./data/", "Smap_embd_block_3sp_pyEDM.csv");

    smapVals = SMap ( "../data/", "block_3sp.csv",
                      "./data/", "Smap_embd_block_3sp_cppEDM.csv",
                      " 1 99 ", "100 198", 3, 1, 0, -1, 2, 0,
                      "x_t y_t z_t", "x_t", "", "", true, false, false );

    cppOutput = smapVals.predictions;
    
    // Comparison
    MakeTest ( "SMap: block_3sp test", pyOutput, cppOutput );    
}

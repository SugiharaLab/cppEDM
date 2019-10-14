// CCM test

#include "TestCommon.h"

int main () {

    // Declare DataFrame to hold the valid output
    DataFrame< double > cppOutput;
    DataFrame< double > output;
    
    //---------------------------------------------------------
    // Load cppEDM valid output
    //---------------------------------------------------------
    cppOutput = DataFrame < double > ( "./data/",
                                       "CCM_anch_sst_cppEDM_valid.csv",
                                       true );  // noTime = true
    
    // Generate new cpp output: sequential sampling for reproduceability
    DataFrame < double > ccmOut = CCM( "../data/",                // pathIn
                                       "sardine_anchovy_sst.csv", // dataFile
                                       "./data/",                 // pathOut
                                       "CCM_anch_sst_cppEDM.csv", //predictFile
                                       3,          // E
                                       0,          // Tp
                                       0,          // knn
                                       1,          // tau
                                       "anchovy",  // columns
                                       "np_sst",   // target
                                       "10 75 5",  // libSizes_str
                                       1,          // sample
                                       false,      // random
                                       false,      // replacement
                                       0,          // seed
                                       false );    // verbose
    
    // Load cppEDM output
    output = DataFrame < double > ( "./data/",
                                    "CCM_anch_sst_cppEDM.csv",
                                    true );  // noTime = true
    
    // comparison
    MakeTest ( "CCM sardine_anchovy_sst test", cppOutput, output );
}

// CCM test

#include "TestCommon.h"

int main () {

    // Declare DataFrame to hold the valid output
    DataFrame< double > cppOutput;
    
    //---------------------------------------------------------
    // Load cppEdm output
    //---------------------------------------------------------
    cppOutput = DataFrame < double > ( "./data/",
                                       "CCM_anch_sst_cppEDM_valid.csv",
                                       true );  // noTime = true
    
    //generate new cpp output
    DataFrame < double > output = CCM( "../data/", "sardine_anchovy_sst.csv",
                                       "./data/", "CCM_anch_sst_cppEDM.csv",
                                       3, 0, 0, 1, "anchovy", "np_sst",
                                       "10 75 5", 1, false, 0, true );
    
    // comparison
    MakeTest ("CCM sardine_anchovy_sst test", cppOutput, output );
}

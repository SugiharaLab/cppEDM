//file to test the ccm algo against pyedm

#include "TestCommon.h"

//ccm params for reference
/*
 DataFrame <double > CCM( std::string pathIn,
                          std::string dataFile,
                          std::string pathOut,
                          std::string predictFile,
                          int         E,
                          int         Tp,
                          int         knn,
                          int         tau,
                          std::string columns,
                          std::string target,
                          std::string libSizes_str,
                          int         sample,
                          bool        random,
                          unsigned    seed,
                          bool        embedded,
                          bool        verbose )
   */

int main () {

    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > pyOutput;
    DataFrame< double > cppOutput;
    
    //---------------------------------------------------------
    // Load pyEdm output from:
    // ./CCM.py -i sardine_anchovy_sst.csv -c anchovy -r np_sst -E 3 -L 10 75 5 
    //---------------------------------------------------------
    pyOutput = DataFrame < double > ( "./data/", "CCM_anch_sst_pyEDM.csv" );
    
    //generate cpp output
    cppOutput = CCM( "../data/", "sardine_anchovy_sst.csv",
                     "./data/", "CCM_anch_sst_cppEDM.csv",
                     3, 0, 0, 1, "anchovy", "np_sst", "10 75 5", 1,
                     false, 0, false, true );
    //run comparison
    MakeTest ("sardine_anchovy_sst test", pyOutput, cppOutput );
}

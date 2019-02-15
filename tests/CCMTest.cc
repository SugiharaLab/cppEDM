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

    //to hold the pyEdm command and relevant args
    const std::string pyCCMCmd ("pyEDM/CCM.py ");
    std::string pyCmd;

    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > pyOutput;
    DataFrame< double > cppDf;
    
    ////////////////////////////////
    // test ccm prediction on sardine set with not random
    ////////////////////////////////
    //generate pyEdm output
    pyCmd = pyCCMCmd + " -pa " + dataPath + " -i sardine_anchovy_sst.csv" + 
        " -r np_sst -c anchovy -L 10 70 10 -s 100 "  + 
        " -E 3 -o " + pyOutputPath; 
    std::cout<<pyCmd<<std::endl;
    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = CCM( dataPath, "sardine_anchovy_sst.csv", cppOutputPath, cppOutputFile,
                 3, 0, 0, 1, "anchovy", "np_sst", "10 70 10", 100,
                 false, 0, false, true );
    //run comparison
    MakeTest ("sardine_anchovy_sst.csv test", pyOutput, cppDf);

    ////////////////////////////////
    // test ccm prediction on sardine set with random but same seed obviously
    ////////////////////////////////
    //generate pyEdm output
    pyCmd = pyCCMCmd + " -pa " + dataPath + " -i sardine_anchovy_sst.csv" + 
        " -r np_sst -c anchovy -L 10 70 10 -s 100 -R -S 5"  + 
        " -E 3 -o " + pyOutputPath; 
    std::cout<<pyCmd<<std::endl;
    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = CCM( dataPath, "sardine_anchovy_sst.csv", cppOutputPath, cppOutputFile,
                 3, 0, 0, 1, "anchovy", "np_sst", "10 70 10", 100,
                 true, 5, false, true );
    //run comparison
    MakeTest ("sardine_anchovy_sst.csv with seed test", pyOutput, cppDf);

}

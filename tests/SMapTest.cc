//file to test the smap algo against pyedm

#include "TestCommon.h"

//smap params for reference
/*
   DataFrame< double > data,
   std::string pathOut,
   std::string predictFile,
   std::string lib,
   std::string pred,
   int         E,
   int         Tp,
   int         knn,
   int         tau,
   double      theta,
   std::string columns,
   std::string target,
   std::string smapFile,
   std::string jacobians,
   bool        embedded,
   bool        verbose
   */

int main () {

    //to hold the pyEdm command and relevant args
    const std::string pySMapCmd ("pyEDM/Predict.py -m smap ");
    std::string pyCmd;

    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > pyOutput;
    DataFrame< double > cppDf;
    SMapValues smapVals;
    
    ////////////////////////////////
    // test smap with simple circle test
    //  - already embedded 
    ////////////////////////////////
    //generate pyEdm output
    pyCmd = pySMapCmd + " -pa " + dataPath + " -i circle.csv" + 
        " -r x -c x y -l 1 100 -p 101 198" + 
        " -E 2 -k 0 -u 1 -T 1 -k 97 -t 4 -e -v " + " -o " + pyOutputPath; 
    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = DataFrame < double > (dataPath, "circle.csv");
    smapVals = SMap (cppDf, cppOutputPath, cppOutputFile, " 1 100 ", "101 198", 2, 1, 0, 1, 4, " x y ", "x", "", "", true, true);
    cppDf = smapVals.predictions;
    //run comparison
    MakeTest ("circle.csv test", pyOutput, cppDf);

}

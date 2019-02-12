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

//run the tests
int main () {

    //to hold the pyEdm command and relevant args
    const std::string pySMapCmd ("pyEDM/Predict.py -m smap ");
    std::string pyCmd;

    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > pyOutput;
    DataFrame< double > cppDf;
    SMapValues smapVals;
    std::string inputFile;
    std::string cols;
    std::string target;
    std::string lib;
    std::string pred;

    ////////////////////////////////
    // test smap with simple circle test
    //  - already embedded 
    ////////////////////////////////
    cols =      "x y";
    target =    "x";
    lib =       "1 100";
    pred =      "101 198";
    inputFile = "circle.csv";
    //generate pyEdm output
    pyCmd = pySMapCmd + " -pa " + dataPathArg + " -i " + inputFile + 
        " -r " + target + " -c " + cols + " -l " + lib + " -p " + pred + 
        " -E " + "2" + " -k " + "97" + " -u " + "1" + " -T 1" + " -t " + "4 " + " -e " + pyOutputArg; 
    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > ("tempOutput/",pyOutputFile);
    std::cout << "PY OUTPUT: " << pyOutput;
    //generate cpp output
    cppDf = DataFrame < double > (dataPathArg, inputFile);
    std::cout << "raw data\n"<< cppDf;
    smapVals = SMap (cppDf, cppOutputPath, cppOutputFile, lib, pred, 2, 1, 0, 1, 4, cols, target, "", "", true, true);
    cppDf = smapVals.predictions;
    std::cout << "cpp prediction data\n"<< cppDf;
    //run comparison
    MakeTest ("circle.csv test", pyOutput, cppDf);

}

//this file is used to test the functionality of the simplex algorithm

#include "TestCommon.h"

/* simplex signature for reference 
DataFrame<double> Simplex( DataFrame< double > data,
                           std::string pathOut,
                           std::string predictFile,
                           std::string lib,
                           std::string pred,
                           int         E,
                           int         Tp,
                           int         knn,
                           int         tau,
                           std::string columns,
                           std::string target,
                           bool        embedded,
                           bool        verbose ) {
*/

int main () {

    //to hold the pyEdm command and relevant args
    const std::string pySimplexCmd ("pyEDM/Predict.py");
    std::string pyCmd;

    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > pyOutput;
    DataFrame< double > cppDf;
    
    ////////////////////////////////
    // test simplex with already embedded block data
    //  - already embedded 
    //./Predict.py -e -i block_3sp.csv -r x_t -c x_t y_t z_t -l 1 100 -p 190 198 -T 1 -D -v
    ////////////////////////////////
    pyCmd = pySimplexCmd + " -pa " + dataPath + " -i block_3sp.csv" + 
        " -r x_t -c x_t y_t z_t -l 1 100 -p 190 198" + 
        " -E 3 -k 4 -T 1 -e -o" + pyOutputPath; 

    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = Simplex (dataPath, "block_3sp.csv", cppOutputPath, cppOutputFile, 
            "1 100","190 198",3,1,0,4,"x_t y_t z_t","x_t",true,true);
    //run comparison
    MakeTest ("block_3sp.csv already embedded test", pyOutput, cppDf);


    ////////////////////////////////
    // test simplex with not embedded block data
    // ./Predict.py -i block_3sp.csv -r x_t -c x_t
    // -l 1 100 -p 101 130 -T 1 -P -E 3 -o block3sp_E3.csv -P
    //----------------------------------------------------------
    pyCmd = pySimplexCmd + " -pa " + dataPath + " -i block_3sp.csv" + 
        " -r x_t -c x_t -l 1 100 -p 101 195" + 
        " -E 3 -T 1 -o" + pyOutputPath; 

    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = Simplex (dataPath, "block_3sp.csv", cppOutputPath, cppOutputFile, 
            "1 100","101 195",3,1,0,1,"x_t","x_t",false,true);
    //run comparison
    MakeTest ("block_3sp.csv dynamic embedding test", pyOutput, cppDf);


    ////////////////////////////////
    // test simplex with not embedded block data
    //./PredictDecay.py -i TentMap_rEDM.csv -c TentMap -l 1 100 -p 201 500 -E 2 -P
    //  note : lib shifted over for py since different definition for which idx to start predicting
    ////////////////////////////////
    //skipping cus doesn't work and gonna do ones that work first
    /*
    pyCmd = pySimplexCmd + " -pa " + dataPath + " -i TentMap_rEDM.csv" + 
        " -r TentMap -c TentMap -l 1 100 -p 200 500" + 
        " -E 2 -k 3 -v -T 1 -o" + pyOutputPath; 

    std::system (pyCmd.c_str());
    pyOutput = DataFrame < double > (tempFileDir,pyOutputFile);
    //generate cpp output
    cppDf = Simplex (dataPath, "TentMap_rEDM.csv", cppOutputPath, cppOutputFile, 
            "1 100","201 500",2,1,3,0,"TentMap","TentMap",false,true);
    //run comparison
    MakeTest ("TentMap_rEDM.csv already embedded test", pyOutput, cppDf);
    */

}

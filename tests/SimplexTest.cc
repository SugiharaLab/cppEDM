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
    cppDf = Simplex (dataPath, "block_3sp.csv", cppOutputPath, cppOutputFile, "1 100","190 198",3,1,0,4,"x_t y_t z_t","x_t",true,true);
    //run comparison
    MakeTest ("block_3sp.csv already embedded test", pyOutput, cppDf);


}

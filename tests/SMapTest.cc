//file to test the smap algo against pyedm

#include "TestCommon.h"

std::string pySMapCmd ("pyEDM/Predict.py -m smap ");

//run the tests
int main () {

    //to hold the pyEdm command and relevant args
    std::string pyCmd;
    //temp to hold the csv data for cpp and pyEdm compare
    DataFrame< double > d1;
    DataFrame< double > d2;

    ////////////////////////////////
    //test smap with block 3sp data:
    //  - already embedded 
    ////////////////////////////////

    //generate pyEdm output
    pyCmd = pySMapCmd + dataPathArg + " -i block_3sp.csv" + " -e " + " -r x_t -c x_t y_t z_t -l 1 99 -p 100 198 -T 1 -t 2 " + pyOutputArg; 
    std::system (pyCmd.c_str());
    //load in and compare
    d1 = DataFrame < double > ("tempOutput/","pyOutput.csv");
    std::cout << d1;
    d2 = DataFrame < double > ("../data/","block_3sp.csv");
    std::cout << d2;


}

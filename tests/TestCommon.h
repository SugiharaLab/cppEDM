//this file just provides some testing functions to be used in other testers

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include "Common.h"

#define STR_LINE_SEP    "~~~~~~~~~~~~~~~~~~~~~~~"
#define TAB_CHAR        '\t'            


const float EPSILON = .2;
//consts for using pyEDM paths
const std::string dataPath          = "../data/";
const std::string tempFileDir       = "tempOutput/"; 
//  pyOutputPath is relative to the data path for pyEDM 
const std::string pyOutputFile      = "pyOutput.csv"; 
const std::string pyOutputPath      = "../tests/" +tempFileDir + pyOutputFile; 
const std::string cppOutputPath     = "../tests/"+tempFileDir; 
const std::string cppOutputFile     = "cppOutput.csv"; 

//consts for different output color
const std::string RED_TEXT("\033[0;31m");
const std::string GREEN_TEXT("\033[1;32m");
const std::string YELLOW_TEXT("\033[1;33m");
const std::string RESET_TEXT("\033[0m");


//----------------------------------------------------------------
// Method to check whether two DataFrames are equal
// @param data1:    first DataFrame to check
// @param data2:    second DataFrame to check
// @param fatalErr: set to nonzero if dimensions are not the same
// @return a list of the rows of the dataframe that were different
//----------------------------------------------------------------
std::vector< int > CheckDataFrameEquality (DataFrame< double > data1, 
                                           DataFrame< double > data2,
                                           int & fatalErr) {
    //to keep track of the rows with different content
    std::vector< int > badRows;

    //to hold each row for comparison
    std::valarray< double > row1;
    std::valarray< double > row2;

    //basic check if dimensions are even equal
    if (data1.NRows() != data2.NRows() || data1.NColumns() != data2.NColumns()) {
        fatalErr = 1;
    }

    //check if any rows were different in the dataframes passed in
    for (size_t rowIdx = 0; rowIdx < data1.NRows(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < data1.NColumns(); colIdx++) {

            float diff = std::abs(data1(rowIdx,colIdx) - data2(rowIdx,colIdx));

            if ( diff >= EPSILON ) {
                badRows.push_back( rowIdx );
                break;
            }

        }    
    }

    return badRows;
}

//----------------------------------------------------------------
// Method to format the output for a test
// @param testName: description of the test being run
// @param data1: first DataFrame to check
// @param data2: second DataFrame to check
// @return: none, just prints to stdout
//----------------------------------------------------------------
void MakeTest (std::string testName, DataFrame< double > data1, 
                                     DataFrame< double > data2 ) {

    int fatalErr = 0;

    //the rows that were different in the test
    std::vector< int > badRows = CheckDataFrameEquality (data1, data2, fatalErr);

    if (fatalErr) {
        std::cout << RED_TEXT << 
           "FATAL ERR: dimensions of block 1: " <<
           data1.NRows()<<"x"<<data1.NColumns()<< 
           " DNE dimensions of block 2: " <<
           data2.NRows()<<"x"<<data2.NColumns()
           <<std::endl;
    }

    //print info on test name and test results

    std::cout << STR_LINE_SEP << std::endl;
    std::cout << "Test: " << testName << std::endl;
    std::cout << STR_LINE_SEP << std::endl;

    if ( badRows.empty() ) {
        std::cout << GREEN_TEXT; 
        std::cout << TAB_CHAR << "TEST PASSED. All rows same.";
    }
    else {
        int numBadRows = std::count_if(badRows.begin(), 
                badRows.end(), [](int i){return i != 0;});

        if ( numBadRows < 5) {
            std::cout << YELLOW_TEXT; 
            std::cout << TAB_CHAR << "TEST MARGINALLY FAILED. " ;
        }
        else {
            std::cout << RED_TEXT; 
            std::cout << TAB_CHAR << "TEST FAILED. " ;
        }
        std::cout << numBadRows << " rows different ";
        std::cout << std::endl;

#ifdef PRINT_DIFFERENCE_IN_RESULTS
        std::cout << TAB_CHAR << TAB_CHAR << "Block 1 column names: ";
        for (auto colName : data1.ColumnNames()) { 
            std::cout << std::setw(10) << colName << TAB_CHAR;
        }
        std::cout << std::endl;
        std::cout << TAB_CHAR << TAB_CHAR << "Block 2 column names: ";
        for (auto colName : data2.ColumnNames()) { 
            std::cout << std::setw(10) << colName << TAB_CHAR;
        }
        std::cout << std::endl;

        std::cout << TAB_CHAR << "first 10 different rows:" << std::endl;
        
        for (auto iterate = badRows.begin(); 
                iterate - badRows.begin() < 10 && iterate != badRows.end(); 
                ++iterate) {

            std::valarray< double > badRow1 = data1.Row( *iterate );
            std::valarray< double > badRow2 = data2.Row( *iterate );

            std::cout << TAB_CHAR << TAB_CHAR << "Block 1 row "<< *iterate << ": ";
            for (double elem : badRow1) {
                std::cout << std::setw(10) << TAB_CHAR << elem << " ";
            }
            std::cout << std::endl;
            std::cout << TAB_CHAR << TAB_CHAR << "Block 2 row "<< *iterate << ": ";
            for (double elem : badRow2) {
                std::cout << std::setw(10) << TAB_CHAR << elem << " ";
            }
            std::cout << std::endl << std::endl;
            
        }

#endif
    }
    
    std::cout << RESET_TEXT << std::endl << std::flush;
}

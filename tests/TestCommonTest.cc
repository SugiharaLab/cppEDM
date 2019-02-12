//this files is used to test the functionality of the testing code. LOL.
//might seem redundant but if somebody thinks there's a discrepancy w the testing
//code this is the place to check

#include "TestCommon.h"

//run the tests
int main () {

    //test with same data
    DataFrame< double > d1 ("../data/","SampleCSV.csv");
    DataFrame< double > d2 ("../data/","SampleCSV.csv");
    MakeTest ("test dfs with same data",d1, d2);

    //test with different data
    d1 = DataFrame< double > ("../data/","SampleCSV.csv");
    d2 = DataFrame< double > ("../data/","SampleCSV.csv");
    d1(4,4) = -5;
    MakeTest ("test dfs w different data",d1, d2);

    //test with wrong dimensions
    d1 = DataFrame< double >("../data/","SampleCSV.csv");
    d2 = DataFrame< double >(1,1);
    MakeTest ("test dfs w different dimensions",d1, d2);
}

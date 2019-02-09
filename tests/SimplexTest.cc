//this file is used to test the functionality of the simplex algorithm

#include "TestCommon.h"

//run the tests
int main () {
    DataFrame< double > d1 ("../data/","SampleCSV.csv");
    DataFrame< double > d2 ("../data/","SampleCSV.csv");
    MakeTest ("test dfs",d1, d2);
}

// Test functionality of the testing code.

#include "TestCommon.h"

//run the tests
int main () {

    //test with same data
    DataFrame< double > d1 ("../data/","block_3sp.csv");
    DataFrame< double > d2 ("../data/","block_3sp.csv");
    MakeTest ( "Data Frames with same data.", d1,  d2 );

    //test with different data
    d1 = DataFrame< double > ("../data/","block_3sp.csv");
    d2 = DataFrame< double > ("../data/","block_3sp.csv");
    d1(4,4) = -5;
    MakeTest ( "Data Frames with different data. "
               "Expect MARGINALLY FAILED.", d1,  d2 );

    //test with wrong dimensions
    d1 = DataFrame< double >("../data/","sardine_anchovy_sst.csv");
    d2 = DataFrame< double >(3,3);
    MakeTest ( "Data Frames with different dimensions. "
               "Expect TEST FAILED.", d1, d2 );
}

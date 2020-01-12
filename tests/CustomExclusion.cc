// custom exclusion data test

#include "TestCommon.h"
#include <vector>
#include "Embed.cc"

using namespace std;

int main () {

    DataFrame < double > twospike_data( "data/", "twospike_data.csv" );

    vector< vector<bool> > exclusion_mat( twospike_data.NRows(), 
                                    vector<bool> (twospike_data.NRows(),false));

    // want to exclude indices 20-32 when predicting indices 72-84 (non-1 vals)

    for ( int j = 72; j < 83; j++ )
        for ( int i = 20; i < 32; i++ )
            exclusion_mat[j][i] = true;

    // try predicting on data now without non-1 points
    
    cout<< Simplex( twospike_data, "","","1 33","70 80",3,1,4,3,
                                0, &exclusion_mat,
                                "wave","wave", false,false,false);

}

// Program to test the conditional embeddings functionality
// No ground truth to test on, so just checking for valid functionality here

#include <iostream>
#include <string>
#include "TestCommon.h" 

int main() {

    DataFrame < double > circleData( "../data/", "circle.csv" );

    size_t num_rows = 30;

    std::vector<bool> valid_lib(num_rows,1);
    for(int i=10; i <20;i++){
        valid_lib[i]=false;
    }

    SMapValues smapVals = SMap ( circleData,
                                 "./data/", "",
                                 " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "Smap_circle_coef.csv", "",
                                 true, false, false, valid_lib );
    auto cppOutput = Simplex ( "../data/", "block_3sp.csv",
                          "./data/", "Smplx_embd_block_3sp_cppEDM.csv",
                          "1 99","100 198", 3, 1, 0, -1, 0,
                          "x_t y_t z_t", "x_t", true, false, false);
 
    
}

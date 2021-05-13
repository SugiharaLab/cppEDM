// Test conditional embedding functionality

#include <iostream>
#include <string>
#include "TestCommon.h" 

int main() {

    DataFrame < double > circleData( "../data/", "circle.csv" );

    size_t num_rows = circleData.NRows();

    //-------------------------------------------------------------------
    // Test of CE validLib as all true... should be no change
    //-------------------------------------------------------------------
    std::vector<bool> valid_lib( num_rows, true );

    auto fullLib = Simplex ( "../data/", "block_3sp.csv", "./data/", "",
                             "1 99","100 198", 3, 1, 0, -1, 0,
                             "x_t y_t z_t", "x_t", true, false, false );

    auto CELib   = Simplex ( "../data/", "block_3sp.csv", "./data/", "",
                             "1 99","100 198", 3, 1, 0, -1, 0,
                             "x_t y_t z_t", "x_t", true, false, false,
                             valid_lib );

    // Should PASS
    MakeTest ( "Simplex: CE full valid", fullLib, CELib );

    // Test of CE validLib as all true... should be no change
    SMapValues fullSMap = SMap ( circleData, "./data/", "",
                                 "1 100", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "", "",
                                 true, false, false );

    SMapValues CESMap   = SMap ( circleData, "./data/", "",
                                 "1 100", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "", "",
                                 true, false, false, valid_lib );

    // Should PASS
    MakeTest ( "SMap: CE full valid", fullSMap.predictions, CESMap.predictions );

    //-------------------------------------------------------------------
    // Remove validLib rows.  valid_lib [0:9] = true
    //-------------------------------------------------------------------
    for( int i = 10; i < num_rows; i++ ) {
        valid_lib[i] = false;
    }

    //-------------------------------------------------------------------
    // Limit lib to the same as valid_lib *** NOTE: lib = "1 11" works
    //-------------------------------------------------------------------
    auto partLib = Simplex ( "../data/", "block_3sp.csv", "./data/", "",
                             "1 10","100 198", 3, 1, 0, -1, 0,
                             "x_t y_t z_t", "x_t", true, false, false );

    // Run with valid_lib that is 0-9
    CELib = Simplex ( "../data/", "block_3sp.csv", "./data/", "",
                      "1 99","100 198", 3, 1, 0, -1, 0,
                      "x_t y_t z_t", "x_t", true, false, false,
                      valid_lib );

    // Should PASS
    MakeTest ( "Simplex: Part valid", CELib, partLib );
    
    //-------------------------------------------------------------
    // CE SMap  *** NOTE: lib = "1 11" works
    //-------------------------------------------------------------
    SMapValues SMapPart = SMap ( circleData, "./data/", "",
                                 "1 10", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "", "",
                                 true, false, false, valid_lib );

    // Run with valid_lib that is 0-9
    CESMap = SMap ( circleData, "./data/", "",
                    "1 100", "101 198", 2, 1, 0, -1, 4, 0,
                    "x y", "x", "CELib_Smap_circle_coef.csv", "",
                    true, false, false, valid_lib );

    // Should PASS
    MakeTest ( "SMap: Part valid", CELib, partLib );
    
#ifdef RUN_ALL
    // Should be no differences in coefficients
    auto coefficients1 = SMapPart.coefficients.VectorColumnName( "C0" );
    auto coefficients2 = CESMap.coefficients.VectorColumnName  ( "C0" );
    
    std::cout << std::endl << "SMap: Partial library coefficients C0:\n\t";
    for ( auto i = 90; i < 99; i++ ) {
        std::cout << coefficients1[ i ] << " ";
    }
    std::cout << std::endl << "SMap: CE library coefficients C0:\n\t";
    for ( auto i = 90; i < 99; i++ ) {
        std::cout << coefficients2[ i ] << " ";
    } std::cout << std::endl;
#endif
}

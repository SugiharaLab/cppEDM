// Program to test the conditional embeddings functionality
// No ground truth to test on, so just checking for valid functionality here

#include <iostream>
#include <string>
#include "TestCommon.h" 

int main() {

    DataFrame < double > circleData( "../data/", "circle.csv" );

    size_t num_rows = circleData.NRows();

    // Test of CE validLib as all true... should be no change
    std::vector<bool> valid_lib( num_rows, true );

    auto fullLib = Simplex ( "../data/", "block_3sp.csv",
                             "./data/", "",
                             "1 99","100 198", 3, 1, 0, -1, 0,
                             "x_t y_t z_t", "x_t", true, false, false );

    auto CELib = Simplex ( "../data/", "block_3sp.csv",
                           "./data/", "",
                           "1 99","100 198", 3, 1, 0, -1, 0,
                           "x_t y_t z_t", "x_t", true, false, false,
                           valid_lib );

    // Should PASS
    MakeTest ( "Simplex: CE full valid", fullLib, CELib );

    SMapValues fullSMap = SMap ( circleData,
                                 "./data/", "",
                                 " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "FullLib_Smap_circle_coef.csv", "",
                                 true, false, false );

    SMapValues CESMap = SMap ( circleData,
                               "./data/", "",
                               " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                               "x y", "x", "", "",
                               true, false, false, valid_lib );

    // Should PASS
    MakeTest ( "SMap: CE full valid", fullSMap.predictions, CESMap.predictions );

    // Remove validLib rows
    for( int i = 10; i < 189; i++ ) {
        valid_lib[i] = false;
    }

    CELib = Simplex ( "../data/", "block_3sp.csv",
                      "./data/", "",
                      "1 99","100 198", 3, 1, 0, -1, 0,
                      "x_t y_t z_t", "x_t", true, false, false,
                      valid_lib );

    // Should be differences in prediction
    std::valarray< double > predictions =
        fullLib.VectorColumnName( "Predictions" );
    
    std::valarray< double > CEpredictions =
        CELib.VectorColumnName( "Predictions" );
    
    std::cout << std::endl << "Simplex: Full library predictions:\n\t";
    for ( auto i = 0; i < 9; i++ ) {
        std::cout << predictions[ i ] << " ";
    }
    std::cout << std::endl << "Simplex: CE library predictions:\n\t";
    for ( auto i = 0; i < 9; i++ ) {
        std::cout << CEpredictions[ i ] << " ";
    } std::cout << std::endl;

    // CE SMap 
    CESMap = SMap ( circleData,
                    "./data/", "",
                    " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                    "x y", "x", "CELib_Smap_circle_coef.csv", "",
                    true, false, false, valid_lib );

    // Should be differences in prediction
    predictions   = fullSMap.predictions.VectorColumnName( "Predictions" );
    CEpredictions = CESMap.predictions.VectorColumnName  ( "Predictions" );
    
    std::cout << std::endl << "SMap: Full library predictions:\n\t";
    for ( auto i = 90; i < 99; i++ ) {
        std::cout << predictions[ i ] << " ";
    }
    std::cout << std::endl << "SMap: CE library predictions:\n\t";
    for ( auto i = 90; i < 99; i++ ) {
        std::cout << CEpredictions[ i ] << " ";
    } std::cout << std::endl;

}

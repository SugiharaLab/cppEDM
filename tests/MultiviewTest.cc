// Multiview Test

#include "TestCommon.h"

int main( int argc, char *argv[] ) {

    //---------------------------------------------------------
    // Load valid cppEDM output
    //---------------------------------------------------------
    DataFrame < double > validCppPredict( "./data/",
                                          "Multiview_pred_valid.csv" );
    
    DataFrame < double > validCppCombos( "./data/",
                                         "Multiview_combos_valid.csv",
                                         true ); // noTime = true
    
    // Generate new cppEDM output
    MultiviewValues MV = Multiview( "../data/",
                                    "block_3sp.csv",
                                    "./",
                                    "MultiviewBlock3sp.csv",
                                    "1 100",
                                    "101 198",
                                    3,             // E
                                    1,             // Tp
                                    0,             // knn
                                    1,             // tau
                                    "x_t y_t z_t", // columns
                                    "x_t",         // target,
                                    0,             // multiview
                                    false,         // verbose,
                                    1 );           // nThreads

    DataFrame< double > combos = MV.Combo_rho;
    DataFrame< double > output = MV.Predictions;

    combos.WriteData( "./", "Multiview_combos_valid.csv" );
    
    //VectorError ve = ComputeError( output.VectorColumnName("Observations"),
    //                               output.VectorColumnName("Predictions"));
    
    //std::cout << "Multiview(): rho " << ve.rho
    //          << "  MAE " << ve.MAE << "  RMSE " << ve.RMSE << std::endl;

    // comparison
    MakeTest ("Multiview combos test",     validCppCombos,  combos );
    MakeTest ("Multiview prediction test", validCppPredict, output );
    
    return 0;
}

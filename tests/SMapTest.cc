// SMap test

#include "TestCommon.h"

int main () {

    //---------------------------------------------------------
    // test smap with simple circle test
    //   embedded = true
    //---------------------------------------------------------
    // Load validation
    //---------------------------------------------------------
    DataFrame < double > coefOutput( "./data/", "Smap_circle_coef_valid.csv" );

    // Generate cpp output
    DataFrame < double > circleData( "../data/", "circle.csv" );

    SMapValues smapVals = SMap ( circleData,
                                 "./data/", "",
                                 " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x",
                                 "Smap_circle_coef.csv",
                                 "Smap_circle_SV.csv",
                                 true, false, false );

    DataFrame < double > cppOutput = smapVals.predictions;
    DataFrame < double > cppCoef   = smapVals.coefficients;

    // Comparison
    MakeTest ( "SMap: circle test", coefOutput, cppCoef );


    //---------------------------------------------------------
    // Load validation
    //---------------------------------------------------------
    DataFrame < double > pyOutput =
        DataFrame <double> ("./data/", "Smap_embd_block_3sp_pyEDM.csv");

    smapVals = SMap ( "../data/", "block_3sp.csv",
                      "./data/", "Smap_embd_block_3sp_cppEDM.csv",
                      " 1 99 ", "100 198", 3, 1, 0, -1, 2, 0,
                      "x_t y_t z_t", "x_t", "", "", true, false, false );

    cppOutput = smapVals.predictions;

    // Comparison
    MakeTest ( "SMap: block_3sp test", pyOutput, cppOutput );

    //---------------------------------------------------------
    // SMap nan test : edmNan
    //   ./data/circle.csv "1 20" "1 20" "x" x out.csv 2 1 -1
    //   0 0 smap 2 n y y n n n 0 0 "1 2 13" "1 1 1"
    //---------------------------------------------------------

    // Load coeff validation
    DataFrame < double >  SmapNanCoeff1 =
        DataFrame <double> ("./data/", "SmapNanCoeff1_valid.csv");

    // Load prediction validation
    DataFrame < double >  SmapNanPred1 =
        DataFrame <double> ("./data/", "SmapNanPred1_valid.csv");

    // Load circle data, insert nan at rows 1 2 13, column x
    DataFrame < double > circleNanData( "../data/", "circle.csv" );
    circleNanData( 0,  0 ) = nan("SmapTest");
    circleNanData( 1,  0 ) = nan("SmapTest");
    circleNanData( 12, 0 ) = nan("SmapTest");

    // Smap with nan at rows 1 2 13, column x
    SMapValues smapNanVals = SMap( circleNanData,
                                   "./data/", "Smap_nan_circle_pred.csv",
                                   " 1 20 ", "1 20", 2, 1, 0, -1, 2, 0,
                                   "x", "x",
                                   "Smap_nan_circle_coef.csv",
                                   "Smap_nan_circle_SV.csv",
                                   false, false, false );

    DataFrame < double > cppNanOutput = smapNanVals.predictions;
    DataFrame < double > cppNanCoef   = smapNanVals.coefficients;

    // Comparison
    MakeTest ( "SMap: nan circle test: coeff", cppNanOutput, SmapNanPred1  );
    MakeTest ( "SMap: nan circle test: pred",  cppNanCoef,   SmapNanCoeff1 );

}

// CCM test

#include "TestCommon.h"

int main () {

    // Declare DataFrame to hold the valid output
    DataFrame< double > cppOutput;
    DataFrame< double > output;
    
    //---------------------------------------------------------
    // Load cppEDM valid output
    //---------------------------------------------------------
    cppOutput = DataFrame < double > ( "./data/",
                                       "CCM_anch_sst_cppEDM_valid.csv",
                                       true );  // noTime = true
    
    // Generate new cpp output: sequential sampling for reproduceability
    CCMValues ccmOut = CCM( "../data/",                // pathIn
                            "sardine_anchovy_sst.csv", // dataFile
                            "./data/",                 // pathOut
                            "CCM_anch_sst_cppEDM.csv", //predictFile
                            3,          // E
                            0,          // Tp
                            0,          // knn
                            -1,         // tau
                            "anchovy",  // columns
                            "np_sst",   // target
                            "10 75 5",  // libSizes_str
                            1,          // sample
                            false,      // random
                            false,      // replacement
                            0,          // seed
                            false,      // includeData
                            false );    // verbose
    
    // Load cppEDM output
    output = DataFrame < double > ( "./data/",
                                    "CCM_anch_sst_cppEDM.csv",
                                    true );  // noTime = true
    
    // comparison
    MakeTest ( "CCM sardine_anchovy_sst test", cppOutput, output );
    
    //------------------------------------------------------------------
    // Thrips
    // Create the ccm matrix of the rEDM-tutorial.Rmd vignette
    //------------------------------------------------------------------
    std::vector< std::string > columns;
    std::vector< std::string > targets;
    
    columns.push_back( "Thrips_imaginis" );
    targets.push_back( "maxT_degC"       );
    
    columns.push_back( "Thrips_imaginis" );
    targets.push_back( "Rain_mm"         );
    
    columns.push_back( "Thrips_imaginis" );
    targets.push_back( "Season"          );
    
    columns.push_back( "maxT_degC" );
    targets.push_back( "Rain_mm"   );
    
    columns.push_back( "maxT_degC" );
    targets.push_back( "Season"    );
    
    columns.push_back( "Rain_mm" );
    targets.push_back( "Season"  );

    std::vector< size_t > rows1( { 1, 2, 3, 2, 3, 3 } );
    std::vector< size_t > cols1( { 0, 0, 0, 1, 1, 2 } );
    std::vector< size_t > rows2( { 0, 0, 0, 1, 1, 2 } );
    std::vector< size_t > cols2( { 1, 2, 3, 2, 3, 3 } );
    
    DataFrame< double > ccmMatrix( 4, 4,
                                   "Thrips_imaginis maxT_degC Rain_mm Season" );

    for ( size_t i = 0; i < columns.size(); i++ ) {
        CCMValues ccmThrips = CCM( "../data/",   // pathIn
                                   "Thrips.csv", // dataFile
                                   "./data/",    // pathOut
                                   "",           //predictFile
                                   8,          // E
                                   0,          // Tp
                                   0,          // knn
                                   -1,         // tau
                                   columns[ i ],// columns
                                   targets[ i ],// target
                                   "73 73 10", // libSizes_str
                                   200,        // sample
                                   true,       // random
                                   false,      // replacement
                                   0,          // seed
                                   false,      // includeData
                                   false );    // verbose
        
        // Load cppEDM output
        DataFrame< double > ccmThripsCSV = ccmThrips.AllLibStats;

        double rho1 = ccmThripsCSV( 0, 1 );
        double rho2 = ccmThripsCSV( 0, 2 );

        ccmMatrix( rows1[ i ], cols1[ i ] ) = rho1;
        ccmMatrix( rows2[ i ], cols2[ i ] ) = rho2;
    }

    // ccmMatrix.WriteData( "./data", "Thrips_CMmatrix_valid.csv" );
    
    //---------------------------------------------------------
    // Load cppEDM valid output
    //---------------------------------------------------------
    DataFrame< double > thripsValid =
        DataFrame < double > ( "./data/",
                               "Thrips_CMmatrix_valid.csv",
                               true );  // noTime = true

    // comparison
    MakeTest ( "CCM Thrips test", thripsValid, ccmMatrix );
    // std::cout << ccmMatrix;
}

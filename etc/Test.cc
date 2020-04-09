
// g++ Test.cc -o Test -std=c++11 -I../src -L../lib -lstdc++ -lEDM -lpthread -llapack -O3
// -g -DDEBUG

#include "Common.h"
#include "Neighbors.h"
#include "Embed.h"

//----------------------------------------------------------------
// Suite of tests for API functionality
// Does not evaluate numerical results or accuracy.
//
// After running Test, visual inspection can be done via R:
//    > source( "PlotTest.R" )
//    > Run()
//    > Clean() 
//----------------------------------------------------------------
//#define EMBED_TEST
#define SIMPLEX_TEST1
#define SIMPLEX_TEST2
#define MULTIVIEW_TEST
#define SMAP_TEST1
#define SMAP_TEST2
#define CCM_TEST
#define EMBED_DIMENSION
#define PREDICT_INTERVAL
#define PREDICT_NONLINEAR

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {
#ifdef EMBED_TEST
        //----------------------------------------------------------
        // Embedding of data file
        //----------------------------------------------------------
        DataFrame< double > EmbedFrame = Embed( "../data/", "block_3sp.csv",
                                                2, -1, "x_t y_t z_t", true );
        std::cout << EmbedFrame;
        
        //----------------------------------------------------------
        // Embedding of DataFrame object not read from disk
        // Load data into DataIO dataFrame, then subset dataFrame 
        // based on column names in Parameters, then pass to Embed.
        // Could bypass Parameters and select dataFrame with explicit colNames
        //----------------------------------------------------------
        Parameters param = Parameters( Method::Simplex,
                                       "../data/", "block_3sp.csv", "", "",
                                       "1 100", "190 198", 2, 1, 4, 1, 0, 0,
                                       "x_t y_t z_t" );
        std::cout << param;
        
        DataFrame< double > dio ( param.pathIn, param.dataFile );
        DataFrame< double > D =
            dio.DataFrameFromColumnNames( param.columnNames );
        
        std::cout << "block_3sp.csv Embed():\n";
        std::cout << D;

        DataFrame< double > EmbedFrame2 = Embed(D, 2, -1, "x_t y_t z_t", true);
        std::cout << "block_3sp.csv DataFrame Embed():\n";
        std::cout << EmbedFrame2;
#endif

#ifdef SIMPLEX_TEST1
        //----------------------------------------------------------
        // embedded = true : data file is multivariable embedding
        //----------------------------------------------------------
        DataFrame<double> dataFrame = 
            Simplex( "../data/",           // pathIn
                     "block_3sp.csv",      // dataFile
                     "./",                 // pathOut
                     "cppBlock3sp_Embedded.csv", // predictFile
                     "1 100",              // lib
                     "101 195",            // pred
                     3,                    // E
                     1,                    // Tp
                     0,                    // knn
                     -1,                   // tau
                     0,                    // exclusionRadius
                     "x_t x_t-1 x_t-2",    // columns
                     "x_t",                // target
                     true,                 // embedded
                     false,                // const_predict
                     true );               // verbose 
        std::cout << dataFrame;

        VectorError ve = ComputeError(
            dataFrame.VectorColumnName( "Observations" ),
            dataFrame.VectorColumnName( "Predictions"  ) );

        std::cout << "Simplex embedded block_3sp.csv:\n";
        std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                  << "  MAE " << ve.MAE << std::endl << std::endl;
        
#endif
        
#ifdef SIMPLEX_TEST2
        //----------------------------------------------------------
        // embedded = false : Simplex embeds data file columns to E
        //----------------------------------------------------------
        DataFrame<double> dataFrameEmbed = 
            Simplex( "../data/",           // pathIn
                     "block_3sp.csv",      // dataFile
                     "./",                 // pathOut
                     "cppBlock3sp_E3.csv", // predictFile
                     "1 100",              // lib
                     "101 195",            // pred
                     3,                    // E
                     1,                    // Tp
                     0,                    // knn
                     -1,                   // tau
                     0,                    // exclusionRadius
                     "x_t y_t z_t",        // columns
                     "x_t",                // target
                     false,                // embedded
                     false,                // const_predict
                     true );               // verbose 
        
        dataFrameEmbed.MaxRowPrint() = 12; // Set number of rows to print
        std::cout << dataFrameEmbed;

        VectorError ve2 = ComputeError(
            dataFrameEmbed.VectorColumnName( "Observations" ),
            dataFrameEmbed.VectorColumnName( "Predictions"  ) );
        
        std::cout << "Simplex E=3 block_3sp.csv:\n";
        std::cout << "rho " << ve2.rho << "  RMSE " << ve2.RMSE
                  << "  MAE " << ve2.MAE << std::endl << std::endl;
#endif

#ifdef MULTIVIEW_TEST
        //----------------------------------------------------------
        //----------------------------------------------------------
        MultiviewValues MV =
            Multiview( "../data/",
                       "block_3sp.csv",
                       "./", "MultiviewBlock3sp.csv",
                       "1 100",       // lib
                       "101 198",     // pred
                       3,             // E
                       1,             // Tp
                       0,             // knn
                       -1,            // tau
                       "x_t y_t z_t", // columns
                       "x_t",         // target,
                       0,             // multiview
                       0,             // exclusionRadius
                       false,         // verbose,
                       1 );           // nThreads
        
        DataFrame< double > MVPredictions = MV.Predictions;
        VectorError vemv =
            ComputeError( MVPredictions.VectorColumnName( "Observations" ),
                          MVPredictions.VectorColumnName( "Predictions"  ));

        std::cout << "Multiview() block_3sp.csv:\nrho " << vemv.rho
                  << "  MAE " << vemv.MAE
                  << "  RMSE " << vemv.RMSE << std::endl;
#endif

#ifdef SMAP_TEST1
        //----------------------------------------------------------
        // embedded = false : SMap embeds data file columns to E
        //----------------------------------------------------------
        SMapValues SMV = 
            SMap( "../data/",           // pathIn
                  "block_3sp.csv",      // dataFile
                  "./",                 // pathOut
                  "smap_3sp_Embed.csv", // predictFile
                  "1 100",              // lib
                  "101 195",            // pred
                  3,                    // E
                  1,                    // Tp
                  0,                    // knn
                  -1,                   // tau
                  3.,                   // theta
                  0,                    // exclusionRadius
                  "x_t",                // columns
                  "x_t",                // target
                  "smap_3sp_coeff.csv", // smapFile
                  "",                   // derivatives
                  false,                // embedded
                  false,                // const_predict
                  true );               // verbose

        DataFrame< double > predictions  = SMV.predictions;
        DataFrame< double > coefficients = SMV.coefficients;
        
        std::cout << predictions;

        VectorError vesm = ComputeError(
            predictions.VectorColumnName( "Observations" ),
            predictions.VectorColumnName( "Predictions"  ) );

        std::cout << "SMap E=3 block_3sp.csv:\n";
        std::cout << "rho " << vesm.rho << "  RMSE " << vesm.RMSE
                  << "  MAE " << vesm.MAE << std::endl << std::endl;
#endif

#ifdef SMAP_TEST2
        //----------------------------------------------------------
        // embedded = true : Circle 
        //----------------------------------------------------------
        SMapValues SMV2 = 
            SMap( "../data/",        // pathIn
                  "circle.csv",      // dataFile,
                  "./",              // pathOut
                  "smap_circle.csv", // predictFile
                  "1 100",           // lib
                  "101 198",         // pred
                  2,                 // E
                  1,                 // Tp
                  0,                 // knn
                  -1,                // tau
                  4.,                // theta
                  0,                 // exclusionRadius
                  "x y",             // columns
                  "x",               // target
                  "smap_circ_coeff.csv", // smapFile
                  "",                    // derivatives
                  true,                  // embedded 
                  false,                 // const_predict
                  true );                // verbose 

        DataFrame< double > predictions2  = SMV2.predictions;
        DataFrame< double > coefficients2 = SMV2.coefficients;
        
        std::cout << predictions2;

        VectorError vesm2 = ComputeError(
            predictions2.VectorColumnName( "Observations" ),
            predictions2.VectorColumnName( "Predictions"  ) );

        std::cout << "SMap embedded circle.csv:\n";
        std::cout << "rho " << vesm2.rho << "  RMSE " << vesm2.RMSE
                  << "  MAE " << vesm2.MAE << std::endl << std::endl;
#endif

#ifdef CCM_TEST
        //----------------------------------------------------------
        //----------------------------------------------------------
        CCMValues CCMVals = 
            CCM( "../data/",                // pathIn
                 "sardine_anchovy_sst.csv", // dataFile
                 "./",                      // pathOut
                 "ccm.csv",                 // predictFile
                 3,                         // E
                 0,                         // Tp
                 0,                         // knn
                 -1,                        // tau
                 "anchovy",                 // columns
                 "np_sst",                  // target
                 "10 70 10",                // libSizes_str
                 100,                       // sample
                 true,                      // random
                 false,                     // replacement
                 0,                         // seed
                 true );                    // verbose

        std::cout << "CCM sardine_anchovy_sst.csv:\n";
        std::cout << CCMVals.AllLibStats;
#endif

#ifdef EMBED_DIMENSION
        //----------------------------------------------------------
        //----------------------------------------------------------
        DataFrame< double > EMBD = 
            EmbedDimension( "../data/",         // pathIn
                            "TentMap_rEDM.csv", // dataFile
                            "./",               // pathOut
                            "EmbedDimOut.csv",  // predictFile
                            "1 100",            // lib
                            "201 500",          // pred
                            10,                 // maxE
                            1,                  // Tp
                            -1,                 // tau
                            "TentMap",          // colNames
                            "",                 // targetName
                            false,              // embedded
                            false,              // verbose
                            4 );                // nThreads 
                
        std::cout << "EmbedDimension TentMap_rEDM.csv:\n";
        std::cout << EMBD;
#endif

#ifdef PREDICT_INTERVAL
        //----------------------------------------------------------
        //----------------------------------------------------------
        DataFrame< double > PD = 
            PredictInterval( "../data/",             // pathIn
                             "TentMap_rEDM.csv",     // dataFile
                             "./",                   // pathOut
                             "PredictIntervalOut.csv",// predictFile
                             "1 100",                // lib
                             "201 500",              // pred
                             10,                     // maxTp
                             2,                      // E
                             -1,                     // tau
                             "TentMap",              // colNames
                             "",                     // targetName,
                             false,                  // embedded
                             false,                  // verbose
                             4 );                    // nThreads
                
        std::cout << "PredictInterval TentMap_rEDM.csv:\n";
        std::cout << PD;
#endif

#ifdef PREDICT_NONLINEAR
        //----------------------------------------------------------
        //----------------------------------------------------------
        DataFrame< double > NL =
            PredictNonlinear( "../data/",  // pathIn,
                              "TentMapNoise_rEDM.csv",   // dataFile,
                              "./",        // pathOut,
                              "PredictNonlinearOut.csv", // predictFile,
                              "1 100",     // lib,
                              "201 500",   // pred,
                              "",          // theta,
                              2,           // E,
                              1,           // Tp,
                              0,           // knn,
                              -1,          // tau,
                              "TentMap",   // colNames,
                              "",          // targetName,
                              false,       // embedded,
                              false,       // verbose,
                              4 );         // nThreads

        NL.MaxRowPrint() = 15;
        std::cout << "PredictNonlinear TentMapNoise_rEDM.csv:\n";
        std::cout << NL;
#endif

    }
    
    catch ( const std::exception& e ) {
 	std::cout << "Exception caught in main:\n";
        std::cout << e.what() << std::endl;
	return -1;
    }
    catch (...) {
 	std::cout << "Unknown exception caught in main.\n";
	return -1;
    }

    std::cout << "normal termination\n";
    
    return 0;
}

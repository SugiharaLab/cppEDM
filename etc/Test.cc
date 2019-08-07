
// g++ Test.cc -o Test -std=c++11 -I../src -L../lib -lstdc++ -lEDM -lpthread -O3
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
                                                2, 1, "x_t y_t z_t", true );
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

        DataFrame< double > EmbedFrame2 = Embed( D, 2, 1, "x_t y_t z_t", true );
        std::cout << "block_3sp.csv DataFrame Embed():\n";
        std::cout << EmbedFrame2;
#endif

#ifdef SIMPLEX_TEST1
        //----------------------------------------------------------
        // embedded = true : data file is multivariable embedding
        // ./Predict.py -i block_3sp.csv -r x_t -c x_t x_t-1 x_t-2
        //   -l 1 100 -p 101 130 -T 1 -P -e -o block3sp_Embedded.csv
        //----------------------------------------------------------
        DataFrame<double> dataFrame = 
            Simplex( "../data/", "block_3sp.csv", "./",
                     "cppBlock3sp_Embedded.csv",
                     "1 100", "101 195", 3, 1, 0, 1, 0,
                     "x_t x_t-1 x_t-2", "x_t", true, false, true );
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
        // ./Predict.py -i block_3sp.csv -r x_t -c x_t
        // -l 1 100 -p 101 130 -T 1 -P -E 3 -o block3sp_E3.csv -P
        //----------------------------------------------------------
        DataFrame<double> dataFrameEmbed = 
            Simplex( "../data/", "block_3sp.csv", "./",
                     "cppBlock3sp_E3.csv",
                     "1 100", "101 195", 3, 1, 0, 1, 0,
                     "x_t y_t z_t", "x_t", false, false, true );
        
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
        // ./Multiview.py -i block_3sp.csv -E 3 -r x_t -c x_t y_t z_t
        // -l 1 100 -p 101 198 -T 1 -P
        //----------------------------------------------------------
        MultiviewValues MV =
            Multiview( "../data/", "block_3sp.csv",
                       "./", "MultiviewBlock3sp.csv",
                       "1 100", "101 198",
                       3,             // E
                       1,             // Tp
                       0,             // knn
                       1,             // tau
                       "x_t y_t z_t", // columns
                       "x_t",         // target,
                       0,             // multiview
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
            SMap( "../data/", "block_3sp.csv", "./", "smap_3sp_Embed.csv",
                  "1 100", "101 195", 3, 1, 0, 1, 3., 0, 
                  "x_t", "x_t", "smap_3sp_coeff.csv", "",
                  false, false, true );

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
            SMap( "../data/", "circle.csv", "./", "smap_circle.csv",
                  "1 100", "101 198", 2, 1, 0, 1, 4., 0,
                  "x y", "x", "smap_circ_coeff.csv", "",
                  true, false, true );

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
        // ./CCM.py -i sardine_anchovy_sst.csv -c anchovy -r np_sst
        // -E 3 -s 100 -L 10 80 10 -R
        //----------------------------------------------------------
        DataFrame< double > CCMD = 
            CCM( "../data/", "sardine_anchovy_sst.csv", "./", "ccm.csv",
                 3, 0, 0, 1, "anchovy", "np_sst", "10 80 10", 100,
                 true, 0, true );

        std::cout << "CCM sardine_anchovy_sst.csv:\n";
        std::cout << CCMD;
#endif

#ifdef EMBED_DIMENSION
        //----------------------------------------------------------
        // ./EmbedDimension.py -i TentMap_rEDM.csv -c TentMap
        // -l 1 100 -p 201 500 -T 1
        //----------------------------------------------------------
        DataFrame< double > EMBD = 
            EmbedDimension( "../data/", "TentMap_rEDM.csv",
                            "./", "EmbedDimOut.csv",
                            "1 100", "201 500", 10, 1, 1,
                            "TentMap", "", false, false, 4 );
                
        std::cout << "EmbedDimension TentMap_rEDM.csv:\n";
        std::cout << EMBD;
#endif

#ifdef PREDICT_INTERVAL
        //----------------------------------------------------------
        // ./PredictDecay.py -i TentMap_rEDM.csv -c TentMap
        // -l 1 100 -p 201 500 -E 2
        //----------------------------------------------------------
        DataFrame< double > PD = 
            PredictInterval( "../data/", "TentMap_rEDM.csv",
                             "./", "PredictIntervalOut.csv",
                             "1 100", "201 500", 10, 2, 1,
                             "TentMap", "", false, false, 4 );
                
        std::cout << "PredictInterval TentMap_rEDM.csv:\n";
        std::cout << PD;
#endif

#ifdef PREDICT_NONLINEAR
        //----------------------------------------------------------
        // ./SMapNL.py -i TentMapErr_rEDM.csv -c TentMap
        // -l 1 100 -p 201 500 -T 1 -E 2 
        //----------------------------------------------------------
        DataFrame< double > NL = 
            PredictNonlinear( "../data/", "TentMapNoise_rEDM.csv",
                              "./", "PredictNonlinearOut.csv",
                              "1 100", "201 500", "", 2, 1, 1,
                              "TentMap", "", false, false, 4 );

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

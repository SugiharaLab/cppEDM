
#include "Common.h"
#include "Neighbors.h"
#include "Embed.h"

//#define EMBED_TEST
#define SIMPLEX_TEST1
#define SIMPLEX_TEST2
#define SMAP_TEST

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {
#ifdef EMBED_TEST
        // Embedding of data file
        DataFrame< double > EmbedFrame = Embed( "../data/", "block_3sp.csv",
                                                2, 1, "x_t y_t z_t", true );
        std::cout << EmbedFrame;
        
        // Embedding of DataFrame object not read from disk
        // Load data into DataIO dataFrame, then subset dataFrame 
        // based on column names in Parameters, then pass to Embed.
        // Could bypass Parameters and select dataFrame with explicit colNames
        Parameters param = Parameters( Method::Simplex,
                                       "../data/", "block_3sp.csv", "", "",
                                       "1 100", "190 198", 2, 1, 4, 1, 0,
                                       "x_t y_t z_t" );
        std::cout << param;
        
        DataFrame< double > dio ( param.pathIn, param.dataFile );
        DataFrame< double > D = dio.DataFrameFromColumnNames(
            param.columnNames );
        std::cout << D;

        DataFrame< double > EmbedFrame2 = Embed( D, 2, 1, "x_t y_t z_t", true );
        std::cout << EmbedFrame2;
#endif

#ifdef SIMPLEX_TEST1
        //----------------------------------------------------------
        // embedded = true : data file is multivariable embedding
        //----------------------------------------------------------
        DataFrame<double> dataFrame = 
            Simplex( "../data/", "block_3sp.csv", "./", "simplex_3sp.csv",
                     "1 100", "101 198", 3, 1, 0, 1,
                     "x_t y_t z_t", "x_t", true, true );
        std::cout << dataFrame;

        VectorError ve = ComputeError(
            dataFrame.VectorColumnName( "Observations" ),
            dataFrame.VectorColumnName( "Predictions"  ) );

        std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                  << "  MAE " << ve.MAE << std::endl << std::endl;
        
#endif
        
#ifdef SIMPLEX_TEST2
        //----------------------------------------------------------
        // embedded = false : Simplex embeds data file columns to E
        //----------------------------------------------------------
        DataFrame<double> dataFrameEmbed = 
            Simplex( "../data/", "block_3sp.csv", "./", "simplex_3sp_Embed.csv",
                     "1 100", "101 198", 3, 1, 0, 1,
                     "x_t y_t z_t", "x_t", false, true );
        
        dataFrameEmbed.MaxRowPrint() = 12; // Set number of rows to print
        std::cout << dataFrameEmbed;

        VectorError ve2 = ComputeError(
            dataFrameEmbed.VectorColumnName( "Observations" ),
            dataFrameEmbed.VectorColumnName( "Predictions"  ) );
        
        std::cout << "rho " << ve2.rho << "  RMSE " << ve2.RMSE
                  << "  MAE " << ve2.MAE << std::endl << std::endl;
#endif

#ifdef SMAP_TEST
        //----------------------------------------------------------
        // embedded = false : SMap embeds data file columns to E
        //----------------------------------------------------------
        SMapValues SMV = 
            SMap( "../data/", "block_3sp.csv", "./", "smap_3sp_Embed.csv",
                  "1 100", "101 198", 3, 1, 0, 1, 4.,
                  "x_t y_t z_t", "x_t", "smap_3sp_coeff.csv", "",
                  false, true );

        DataFrame< double > predictions  = SMV.predictions;
        DataFrame< double > coefficients = SMV.coefficients;
        
        predictions.MaxRowPrint() = 10; // Set number of rows to print
        std::cout << predictions;

        VectorError vesm = ComputeError(
            predictions.VectorColumnName( "Observations" ),
            predictions.VectorColumnName( "Predictions"  ) );

        std::cout << "rho " << vesm.rho << "  RMSE " << vesm.RMSE
                  << "  MAE " << vesm.MAE << std::endl << std::endl;
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

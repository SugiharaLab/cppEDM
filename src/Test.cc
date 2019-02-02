
#include "DataIO.h"
#include "Neighbors.h"
#include "Embed.h"

#define SIMPLEX_TEST
//#define EMBED_TEST

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
        
        DataIO dio = DataIO( param.pathIn, param.dataFile );
        DataFrame< double > D = dio.DFrame().DataFrameFromColumnNames(
            param.columnNames );
        std::cout << D;

        DataFrame< double > EmbedFrame2 = Embed( D, 2, 1, "x_t y_t z_t", true );
        std::cout << EmbedFrame2;
#endif

#ifdef SIMPLEX_TEST
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
                  << "  MAE " << ve.MAE << std::endl;
        
        //----------------------------------------------------------
        // embedded = false : Simplex embeds data file columns to E
        //----------------------------------------------------------
        DataFrame<double> dataFrameEmbed = 
            Simplex( "../data/", "block_3sp.csv", "./", "simplex_3sp_Embed.csv",
                     "1 100", "101 198", 3, 1, 0, 1,
                     "x_t y_t z_t", "x_t", false, true );
        
        dataFrameEmbed.MaxRowPrint() = 5; // Set number of rows to print
        std::cout << dataFrameEmbed;

        ve = ComputeError(
            dataFrameEmbed.VectorColumnName( "Observations" ),
            dataFrameEmbed.VectorColumnName( "Predictions"  ) );

        std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                  << "  MAE " << ve.MAE << std::endl;
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


#include "DataFrame.h"
#include "Neighbors.h"
#include "Embed.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {
        //--------------------------------------------------
        // Default parameters
        //--------------------------------------------------
        // Parameters param = Parameters( Method::Simplex,
        //                                "../data/", "block_3sp.csv", "",
        //                                "1 100", "190 198", 3, 1, 4, 1, 0,
        //                                "1 4" );
        // std::cout << param;
        
        // DataFrame df = DataFrame( param.path, param.dataFile );

        // Matrix< double > M = df.DataMatrix();

        // std::cout << M;

        // Matrix< double > EmbedMatrix = Embed ( "../data/", "block_3sp.csv",
        //                                        3, 1, "x_t y_t z_t", true );

        // std::cout << EmbedMatrix;

        Matrix<double> dataMatrix = 
        Simplex( "../data/", "block_3sp.csv", "", "1 100", "190 198",
                 3, 1, 0, 1, "x_t y_t z_t", "x_t", true, true );
        std::cout << dataMatrix;

        // Simplex calls Embed to get dataBlock
        Matrix<double> dataMatrixEmbed = 
        Simplex( "../data/", "block_3sp.csv", "", "1 100", "190 198",
                 3, 1, 0, 1, "x_t y_t z_t", "x_t", false, true );
        std::cout << dataMatrixEmbed;
        
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


#include "DataFrame.h"
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
        Matrix< double > EmbedMatrix = Embed( "../data/", "block_3sp.csv",
                                              2, 1, "x_t y_t z_t", true );
        std::cout << EmbedMatrix;
        // Embedding of Matrix
        // Convoluted to get the dataMatrix to pass to Embed()
        Parameters param = Parameters( Method::Simplex,
                                       "../data/", "block_3sp.csv", "",
                                       "1 100", "190 198", 2, 1, 4, 1, 0,
                                       "x_t y_t z_t" );
        std::cout << param;
        
        DataFrame df = DataFrame( param.path, param.dataFile );
        //std::cout << df;
        Matrix< double > M = df.MatrixColumnNames( param.columnNames );
        std::cout << M;

        Matrix< double > EmbedMatrix2 = Embed( M, 2, 1, "x_t y_t z_t", true );
        std::cout << EmbedMatrix2;
#endif

#ifdef SIMPLEX_TEST
        // embedded = true : data is read from file as embedding
        Matrix<double> dataMatrix = 
        Simplex( "../data/", "block_3sp.csv", "", "1 100", "190 198",
                 3, 1, 0, 1, "x_t y_t z_t", "x_t", true, true );
        std::cout << dataMatrix;

        // embedded = false : Simplex calls Embed to get dataBlock
        Matrix<double> dataMatrixEmbed = 
        Simplex( "../data/", "block_3sp.csv", "", "1 100", "190 198",
                 3, 1, 0, 1, "x_t y_t z_t", "x_t", false, true );
        std::cout << dataMatrixEmbed;
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

#include "Common.h"
#include "Neighbors.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
main( int argc, char *argv[] ) {
    
    try {
        // Silly call to Tokenize() in libEDM on argv
        //--------------------------------------------------
        std::vector<std::string> args( argv, argv + argc );
        
        std::stringstream ssargs;
        for ( auto ai = args.begin(); ai != args.end(); ++ai ) {
            ssargs << *ai << " ";
        }
        
        std::vector<std::string> tokens = Tokenize( ssargs.str() );
        std::cout << "Tokenize( argv ): ";
        for ( auto ai = tokens.begin(); ai != tokens.end(); ++ai ) {
            std::cout << *ai << " ";
        } std::cout << std::endl;

        //--------------------------------------------------
        // Default parameters
        //--------------------------------------------------
        Parameters *parameters = new Parameters;
        parameters->verbose = true;
        parameters->knn     = 2;
        
        //--------------------------------------------------
        // Matrix 6 rows 3 columns
        //--------------------------------------------------
        Matrix<double> M( 6, 3 );
        for ( size_t i = 0; i < 6; i++ ) {
            for ( size_t j = 0; j < 3; j++ ) {
                M( i, j ) = i + j * 2;
            }
        }

        // Call FindNeighbors()
        //--------------------------------------------------
        std::vector<int> libraryRows    = {0,1,2};
        std::vector<int> predictionRows = {2,3,4};

        Neighbors *neighbors = FindNeighbors( &M,
                                              &libraryRows,
                                              &predictionRows,
                                               parameters );
    }
    catch ( const std::exception& e ) {
 	std::cout << "Exception caught in main:\n";
        std::cout << e.what();
	return -1;
    }
    catch (...) {
 	std::cout << "Unknown exception caught in main.\n";
	return -1;
    }

    std::cout << "normal termination\n";
    
    return 0;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
void TestCode( Matrix<double> M ) {
    
        std::valarray<double> A( 99, 3 );
        M.writeRow( 0, A );

        // test Matrix.row() & column()
        std::cout << "Matrix:" << std::endl;
        for ( size_t i = 0; i < 6; i++ ) {
            std::cout << "Row " << i << " : ";
            for ( size_t j = 0; j < 3; j++ ) {
                std::cout << M(i,j) << " ";
            } std::cout << std::endl;
        }   
        std::cout << "Matrix rows():" << std::endl;
        for ( size_t i = 0; i < 6; i++ ) {
            std::valarray<double> row = M.row( i );
            std::cout << "Row " << i << " : ";
            for ( size_t j = 0; j < row.size(); j++ ) {
                std::cout << row[j] << " ";
            } std::cout << std::endl;
        }
        std::cout << "Matrix cols():" << std::endl;
        for ( size_t i = 0; i < 3; i++ ) {
            std::valarray<double> col = M.column( i );
            std::cout << "Column " << i << " : ";
            for ( size_t j = 0; j < col.size(); j++ ) {
                std::cout << col[j] << " ";
            } std::cout << std::endl;
        }

}

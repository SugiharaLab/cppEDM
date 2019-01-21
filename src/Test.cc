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

        // Matrix 
        //--------------------------------------------------
        Matrix<double> M( 6, 3 );
        for ( size_t i = 0; i < 6; i++ ) {
            for ( size_t j = 0; j < 3; j++ ) {
                M( i, j ) = i + j * 2;
            }
        }

        // Default parameters
        Parameters *parameters = new Parameters;
        parameters->verbose = true;
        
        // Call FindNeighbors()
        //--------------------------------------------------
        std::vector<int> libraryRows    = {0,1,2};
        std::vector<int> predictionRows = {2,3,4};

        Neighbors *neighbors = FindNeighbors( &M,
                                              &libraryRows,
                                              &predictionRows,
                                               parameters );
    }
    catch (...) {
 	std::cout << "Exception caught in main.\n";
	return -1;
    }

    std::cout << "normal termination\n";
    
    return 0;
}

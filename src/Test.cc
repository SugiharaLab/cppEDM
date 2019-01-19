#include "Common.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
main( int argc, char *argv[] ) {
    
    try {
        // Silly call to Tokenize() in libEDM on argv
        //--------------------------------------------------
        vector<string> args( argv, argv + argc );
        
        stringstream ssargs;
        for ( auto ai = args.begin(); ai != args.end(); ++ai ) {
            ssargs << *ai << " ";
        }
        
        vector<string> tokens = Tokenize( ssargs.str() );
        cout << "Tokenize( argv ): ";
        for ( auto ai = tokens.begin(); ai != tokens.end(); ++ai ) {
            cout << *ai << " ";
        } cout << endl;

        // Matrix 
        //--------------------------------------------------
        Matrix<double> M( 2, 3 );
        M(0,0) = 11; M(0,1) = 22; M(0,2) = 33;
        M(1,0) = 44; M(1,1) = 55; M(1,2) = 66;
        cout << "M(1,1)=" << M(1,1) << " M(0,2)=" << M(0,2)<< endl;

        // Now call FindNeighbors()
        //--------------------------------------------------
        vector<int> libraryRows    = {1,2,3};
        vector<int> predictionRows = {4,5,6};
        
    }
    catch (...) {
 	cout << "Exception caught in main.\n";
	return -1;
    }

    cout << "normal termination\n";
    
    return 0;
}

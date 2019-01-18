#include "Common.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
main( int argc, char *argv[] ) {
    
    try {
        // Silly example to call Tokenize in libEDM on argv
        vector<string> args( argv, argv + argc );
        
        stringstream ssargs;
        for ( auto ai = args.begin(); ai != args.end(); ++ai ) {
            ssargs << *ai << " ";
        }
        
        vector<string> tokens = Tokenize( ssargs.str() );
        
        for ( auto ai = tokens.begin(); ai != tokens.end(); ++ai ) {
            cout << *ai << " ";
        } cout << endl;
    }
    catch (...) {
 	cout << "Exception caught in main.\n";
	return -1;
    }

    cout << "normal termination\n";
    
    return 0;
}

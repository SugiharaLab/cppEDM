#include "Common.h"

//----------------------------------------------------------------
// Tokenizing function... Creates tokens based on whitespace only
//----------------------------------------------------------------
vector<string> Tokenize( string message ) {
    // Break message string into tokens
    stringstream strstrm( message ); // Insert message into a stringstream
    vector<string> tokens;           // Break by whitespace into tokens
    string tmp_string;
    while ( strstrm >> tmp_string ) {
	tokens.push_back( tmp_string );
    }
    return( tokens );
}

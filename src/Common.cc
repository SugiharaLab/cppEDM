#include "Common.h"


//----------------------------------------------------------------
// Tokenizing function... Creates tokens based on whitespace only
//----------------------------------------------------------------
std::vector<std::string> Tokenize( std::string message ) {
    // Break message string into tokens
    std::stringstream strstrm( message ); // Insert message into a stringstream
    std::vector<std::string> tokens;      // Break by whitespace into tokens
    std::string tmp_string;
    while ( strstrm >> tmp_string ) {
	tokens.push_back( tmp_string );
    }
    return( tokens );
}

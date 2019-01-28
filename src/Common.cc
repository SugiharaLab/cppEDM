
#include <algorithm>

#include "Common.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
std::string ToLower( std::string str ) {

    std::string lowerStr( str );
    std::transform( lowerStr.begin(), lowerStr.end(),
                    lowerStr.begin(), ::tolower );
    
    return lowerStr;
}

//----------------------------------------------------------------
// true : str has only digits   false : str has non-digits
//----------------------------------------------------------------
bool OnlyDigits( std::string str ) {

    if ( not str.size() ) {
        throw std::runtime_error( "OnlyDigits(): String is empty.\n" );
    }

    bool onlyDigits = true; 
    std::string str_( str );
    
    // remove whitespace
    str_.erase( std::remove_if( str_.begin(), str_.end(), ::isspace ),
                str_.end() );

    const char *cstr = str_.c_str(); // isdigit() is a C function...
    
    for ( size_t i = 0; i < str_.size(); i++ ) {
        if ( not isdigit( cstr[i] ) ) { onlyDigits = false; break; }
    }

    return onlyDigits;
}

//----------------------------------------------------------------
// SplitString
//
// Purpose: like Python string.split()
//
// Arguments: inString    : string to be split
//            delimeters  : string of delimeters
//
// Note:  A typical delimeter string: delimeters = " \t,\n:;()"
//           
// Return: vector of tokens
//----------------------------------------------------------------
std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters ) {

  size_t pos       = 0;
  size_t eos       = 0;
  size_t wordStart = 0;
  size_t wordEnd   = 0;

  bool foundStart = false;
  bool foundEnd   = false;

  std::vector<std::string> splitString;
  
  std::string word;

  eos = inString.length();

  while ( pos <= eos ) {
    if ( not foundStart ) {
      if ( delimeters.find( inString[pos] ) == delimeters.npos ) {
	// this char (inString[pos]) is not a delimeter
	wordStart  = pos;
	foundStart = true;
	pos++;
	continue;
      }
    }
    if ( foundStart and not foundEnd ) {
      if ( delimeters.find( inString[pos] ) != delimeters.npos 
	   or pos == eos ) {
	// this char (inString[pos]) is a delimeter or
	// at the end of the string
	wordEnd  = pos;
	foundEnd = true;
      }
    }
    if ( foundStart and foundEnd ) {
      foundStart = false;
      foundEnd   = false;
      
      word = inString.substr( wordStart, wordEnd - wordStart );
      
      // remove whitespace
      word.erase( std::remove_if( word.begin(), word.end(), ::isspace ),
                  word.end() );
      
      splitString.push_back( word );
    }
    if ( pos == eos ) {
      break;
    }
    pos++;
  }

  return splitString;
}

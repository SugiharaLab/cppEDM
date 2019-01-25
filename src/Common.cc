
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

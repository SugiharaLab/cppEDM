#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// Common definitions
typedef map< string, vector< double > > NamedData;

// Tokenize function in Common.cc
vector<string> Tokenize( string message );

#endif

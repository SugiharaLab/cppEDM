#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std; // This is poor usage... should be explicit namespaces.

// Common definitions
typedef map< string, vector< double > > NamedData;
typedef vector< vector< double > >      DataBlock;
typedef vector< vector< int > >         IndexBlock;

enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Tokenize function in Common.cc
vector<string> Tokenize( string message );

#endif

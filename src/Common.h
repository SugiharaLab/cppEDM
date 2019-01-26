#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

// Type definitions
typedef std::vector< std::pair< std::string, std::vector< double > > > NamedData;

// Enumerations
enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Prototypes
//---------------------------------------------------------
std::string ToLower( std::string str );

std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters = "," );

#endif

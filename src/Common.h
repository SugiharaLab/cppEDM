#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <valarray>
#include <map>
#include <cctype>

#include "Matrix.h"

// Type definitions
typedef std::vector<std::pair<std::string, std::vector<double>>> NamedData;

// Enumerations
enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Prototypes
//---------------------------------------------------------
std::string ToLower   ( std::string str );
bool        OnlyDigits( std::string str );

std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters = "," );

Matrix<double> Simplex( std::string path         = "./data/",
                        std::string dataFile     = "block_3sp.csv",
                        std::string predictFile  = "",
                        std::string lib          = "1 10",
                        std::string pred         = "195 198",
                        int         E            = 3,
                        int         Tp           = 1,
                        int         knn          = 0,
                        int         tau          = 1,
                        std::string colNames     = "x_t y_t z_t",
                        std::string targetName   = "x_t",
                        bool        embedded     = true,
                        bool        verbose      = true   );

#endif

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <valarray>
#include <map>
#include <cctype>
#include <cmath>

#include "DataFrame.h" // #include Common.h

// Type definitions
typedef std::vector<std::pair<std::string, std::vector<double>>> NamedData;

// Enumerations
enum class Method         { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Data structs
struct VectorError {
    double rho;
    double RMSE;
    double MAE;
};

// Prototypes
//---------------------------------------------------------
std::string ToLower   ( std::string str );
bool        OnlyDigits( std::string str );

std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters = "," );

VectorError ComputeError( std::valarray< double > obs,
                          std::valarray< double > pred );

DataFrame<double> Simplex( std::string pathIn       = "./data/",
                           std::string dataFile     = "",
                           std::string pathOut      = "./",
                           std::string predictFile  = "",
                           std::string lib          = "1  10",
                           std::string pred         = "11 20",
                           int         E            = 0,
                           int         Tp           = 1,
                           int         knn          = 0,
                           int         tau          = 1,
                           std::string colNames     = "",
                           std::string targetName   = "",
                           bool        embedded     = true,
                           bool        verbose      = true );

DataFrame<double> SMap( std::string pathIn          = "./data/",
                        std::string dataFile        = "",
                        std::string pathOut         = "./",
                        std::string predictFile     = "",
                        std::string lib             = "1  10",
                        std::string pred            = "11 20",
                        int         E               = 0,
                        int         Tp              = 1,
                        int         knn             = 0,
                        int         tau             = 1,
                        double      theta           = 0,
                        std::string columns         = "",
                        std::string target          = "",
                        bool        embedded        = true,
                        bool        verbose         = true,
                        std::string smapFile        = "",
                        std::string jacobians       = "",
                        double      SVDsignificance = 1.E-5 );
#endif

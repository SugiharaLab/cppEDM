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


// Enumerations
enum class Method         { None, Embed, Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Data structs
struct VectorError {
    double rho;
    double RMSE;
    double MAE;
};

struct SMapValues {
    DataFrame< double > predictions;
    DataFrame< double > coefficients;
};

struct CCMResult {
    DataFrame< double > CCM_result;
};

// Prototypes
//---------------------------------------------------------
std::string ToLower   ( std::string str );
bool        OnlyDigits( std::string str );

std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters );

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

SMapValues SMap( std::string pathIn          = "./data/",
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
                 std::string smapFile        = "",
                 std::string jacobians       = "",
                 bool        embedded        = true,
                 bool        verbose         = true );

CCMResult CCM(  std::string pathIn       = "./data/",
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
                std::string libSizes_str = "",
                int         sample       = 0,
                bool        random       = true,
                int         seed         = -1,
                bool        embedded     = true,
                bool        verbose      = true );

#endif

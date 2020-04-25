// File for declarations common to all EDM code

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <valarray>
#include <map>
#include <list>
#include <cctype>
#include <cmath>

#ifdef _MSC_VER
#include <ciso646> // Macro constants for MSVC C++ operators not in ISO646
#endif

#include "DataFrame.h" // Has #include Common.h

// Data structs

struct VectorError {
    double rho;
    double RMSE;
    double MAE;
};

// Enumerations
enum class Method  { None, Embed, Simplex, SMap, CCM };
enum class DistanceMetric { Euclidean, Manhattan };

// Namespace with all functions user should have
namespace cppEDM {

    // Forward declarations of EDM Functions

    DataFrame<double> Simplex( DataFrame<double> & data,
                            std::string pathOut, std::string predictFile,
                            std::string lib, std::string pred,
                            int E, int Tp, int knn, int tau, int exclusionRadius,
                            std::string columns, std::string target,
                            bool embedded, bool const_predict, bool verbose ) ;

} 

#endif

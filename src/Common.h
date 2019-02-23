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

// Normally, macros are eschewed
// Define the initial maximum distance for neigbor distances to avoid sort()
// Note: std::numeric_limits<double>::max() ~1E308
//       std::numeric_limits<float> ::max() ~1E38
//       std::numeric_limits<int64> ::max() ~1E18
//       std::numeric_limits<int>   ::max() ~1E9
#define DISTANCE_MAX   1E300
#define DISTANCE_LIMIT 1E299  // Must be less than DISTANCE_MAX, but large

// Enumerations
enum class Method         { None, Embed, Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

//---------------------------------------------------------
// Data structs
//---------------------------------------------------------
struct VectorError {
    double rho;
    double RMSE;
    double MAE;
};

struct SMapValues {
    DataFrame< double > predictions;
    DataFrame< double > coefficients;
};

//-------------------------------------------------------------
// Prototypes
// Primary API functions generally have two call-signatures.
// The first takes a path, file name pair specifying the data
// file image on disk to be loaded and converted to a data frame.
// The second replaces these two arguments with a DataFrame object.
//
// NOTE: These are the first declarations seen by the compiler
//       for the API and provide default argument values
//-------------------------------------------------------------
std::string ToLower   ( std::string str );
bool        OnlyDigits( std::string str );

std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters );

VectorError ComputeError( std::valarray< double > obs,
                          std::valarray< double > pred );

// API functions Embed() and MakeBlock() are in Embed.h Embed.cc

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
                           bool        embedded     = false,
                           bool        verbose      = true );

DataFrame<double> Simplex( DataFrame< double >,
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
                           bool        embedded     = false,
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
                 bool        embedded        = false,
                 bool        verbose         = true );

SMapValues SMap( DataFrame< double >,
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
                 bool        embedded        = false,
                 bool        verbose         = true );

DataFrame<double> CCM( std::string pathIn       = "./data/",
                       std::string dataFile     = "",
                       std::string pathOut      = "./",
                       std::string predictFile  = "",
                       int         E            = 0,
                       int         Tp           = 0,
                       int         knn          = 0,
                       int         tau          = 1,
                       std::string colNames     = "",
                       std::string targetName   = "",
                       std::string libSizes_str = "",
                       int         sample       = 0,
                       bool        random       = true,
                       unsigned    seed         = 0,     // seed=0: use RNG
                       bool        embedded     = false,
                       bool        verbose      = true );

DataFrame<double> CCM( DataFrame< double >,
                       std::string pathOut      = "./",
                       std::string predictFile  = "",
                       int         E            = 0,
                       int         Tp           = 0,
                       int         knn          = 0,
                       int         tau          = 1,
                       std::string colNames     = "",
                       std::string targetName   = "",
                       std::string libSizes_str = "",
                       int         sample       = 0,
                       bool        random       = true,
                       unsigned    seed         = 0,     // seed=0: use RNG
                       bool        embedded     = false,
                       bool        verbose      = true );

DataFrame<double> EmbedDimension( std::string pathIn      = "./data/",
                                  std::string dataFile    = "",
                                  std::string pathOut     = "./",
                                  std::string predictFile = "",
                                  std::string lib         = "1  10",
                                  std::string pred        = "11 20",
                                  int         Tp          = 1,
                                  int         tau         = 1,
                                  std::string colNames    = "",
                                  std::string targetName  = "",
                                  bool        embedded    = false,
                                  bool        verbose     = true );

DataFrame<double> EmbedDimension( DataFrame< double >,
                                  std::string pathOut     = "./",
                                  std::string predictFile = "",
                                  std::string lib         = "1  10",
                                  std::string pred        = "11 20",
                                  int         Tp          = 1,
                                  int         tau         = 1,
                                  std::string colNames    = "",
                                  std::string targetName  = "",
                                  bool        embedded    = false,
                                  bool        verbose     = true );

DataFrame<double> PredictInterval( std::string pathIn      = "./data/",
                                   std::string dataFile    = "",
                                   std::string pathOut     = "./",
                                   std::string predictFile = "",
                                   std::string lib         = "1  10",
                                   std::string pred        = "11 20",
                                   int         E           = 0,
                                   int         tau         = 1,
                                   std::string colNames    = "",
                                   std::string targetName  = "",
                                   bool        embedded    = false,
                                   bool        verbose     = true );

DataFrame<double> PredictInterval( DataFrame< double >,
                                   std::string pathOut     = "./",
                                   std::string predictFile = "",
                                   std::string lib         = "1  10",
                                   std::string pred        = "11 20",
                                   int         E           = 0,
                                   int         tau         = 1,
                                   std::string colNames    = "",
                                   std::string targetName  = "",
                                   bool        embedded    = false,
                                   bool        verbose     = true );

DataFrame<double> PredictNonlinear( std::string pathIn      = "./data/",
                                    std::string dataFile    = "",
                                    std::string pathOut     = "./",
                                    std::string predictFile = "",
                                    std::string lib         = "1  10",
                                    std::string pred        = "11 20",
                                    int         E           = 0,
                                    int         Tp          = 1,
                                    int         tau         = 1,
                                    std::string colNames    = "",
                                    std::string targetName  = "",
                                    bool        embedded    = false,
                                    bool        verbose     = true );

DataFrame<double> PredictNonlinear( DataFrame< double >,
                                    std::string pathOut     = "./",
                                    std::string predictFile = "",
                                    std::string lib         = "1  10",
                                    std::string pred        = "11 20",
                                    int         E           = 0,
                                    int         Tp          = 1,
                                    int         tau         = 1,
                                    std::string colNames    = "",
                                    std::string targetName  = "",
                                    bool        embedded    = false,
                                    bool        verbose     = true );
#endif

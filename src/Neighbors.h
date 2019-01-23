#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <cmath>

#include "Common.h"
#include "Matrix.h"
#include "Parameter.h"

struct Neighbors; // forward declaration

// Prototypes
struct Neighbors *FindNeighbors( Matrix<double>   *matrix,
                                 std::vector<int> *libraryRows,
                                 std::vector<int> *predictionRows,
                                 Parameters       *parameters );

double Distance( std::valarray<double> *v1,
                 std::valarray<double> *p2,
                 DistanceMetric    metric );

// Return structure of FindNeighbors()
struct Neighbors {
    Matrix<int>    *neighbors;
    Matrix<double> *distances;
    Neighbors();
    ~Neighbors();
};

#endif

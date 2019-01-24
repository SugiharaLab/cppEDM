#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <cmath>
#include <iterator>

#include "Common.h"
#include "Matrix.h"
#include "Parameter.h"

struct Neighbors; // forward declaration

// Prototypes
struct Neighbors FindNeighbors( const Matrix<double> &matrix,
                                const Parameters     &parameters );

double Distance( const std::valarray<double> &v1,
                 const std::valarray<double> &v2,
                 DistanceMetric metric );

// Return structure of FindNeighbors()
struct Neighbors {
    Matrix<int>    neighbors;
    Matrix<double> distances;
    Neighbors();
    ~Neighbors();
};

#endif

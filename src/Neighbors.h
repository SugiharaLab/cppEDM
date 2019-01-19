#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <cmath>

#include "Common.h"

struct Neighbors; // forward declaration

// Prototypes
struct Neighbors FindNeighbors( Matrix<double> *matrix,
                                vector<int>    *libraryRows,
                                vector<int>    *predictionRows );

double Distance( vector<double> *v1,
                 vector<double> *p2,
                 DistanceMetric  metric );

// Return structure of FindNeighbors()
struct Neighbors {
    IndexBlock neighbors;
    DataBlock  distances;
};

#endif

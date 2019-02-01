#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <cmath>
#include <iterator>

#include "Common.h"
#include "Parameter.h"

struct Neighbors; // forward declaration

// Prototypes
struct Neighbors FindNeighbors( const DataFrame<double> &dataFrame,
                                const Parameters        &parameters );

void PrintDataFrameIn( const DataFrame<double> &dataFrame,
                       const Parameters        &parameters );

void PrintNeighborsOut( const Neighbors &neighbors );

double Distance( const std::valarray<double> &v1,
                 const std::valarray<double> &v2,
                 DistanceMetric metric );

// Return structure of FindNeighbors()
struct Neighbors {
    DataFrame<int>    neighbors;
    DataFrame<double> distances;
    Neighbors();
    ~Neighbors();
};

#endif

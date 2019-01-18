
#include "Neighbors.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
struct Neighbors FindNeighbors(
    DataBlock   *data,
    vector<int> *libraryRows,
    vector<int> *predictionRows )
{

    Neighbors neighbors;

    return neighbors;
}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
double Distance( vector<double> *v1,
                 vector<double> *v2,
                 DistanceMetric metric )
{
    double distance = 0;

    // For efficiency sake, we forego the usual validation of v1 & v2.

    if ( metric == DistanceMetric::Euclidean ) {
        // [sum_{i,j} abs(a_{i,j})^2]^{1/2}
        double sum = 0;
        for ( vector<double>::size_type i = 0; i < v1->size(); i++ ) {
            sum += pow( abs( (*v2)[i] - (*v1)[i] ), 2 );
        }
        distance = sqrt( sum );
    }
    else if ( metric == DistanceMetric::Manhattan ) {
        // max( sum( abs(x) ) )
        double sum = 0;
        for ( vector<double>::size_type i = 0; i < v1->size(); i++ ) {
            sum += abs( (*v2)[i] - (*v1)[i] );
        }
        distance = sum;
    }
    else {
        stringstream errMsg;
        errMsg << "Distance() Invalid DistanceMetric: "
               << static_cast<int>( metric );
        throw runtime_error( errMsg.str() );
    }

    return distance;
}

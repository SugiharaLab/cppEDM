
#include "Neighbors.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
struct Neighbors FindNeighbors(
    Matrix<double> *matrix, // 
    vector<int>    *libraryRows,
    vector<int>    *predictionRows )
{

    int library_rows    = libraryRows->size();
    int prediction_rows = predictionRows->size();
    
#ifdef DEBUG
    int Ncol = data->size();
    cout << "Neighbors(): prediction:" << endl;
    for ( size_t row = 0; row < 5; row++ ) {
        int row_i = (*libraryRows)[row];
        for ( size_t col = 0; col < Ncol; col++ ) {
            cout << data[row_i][col] << " "; 
        } cout << endl;
    }
#endif

    // Identify degenerate library : prediction points by time comparison
    //vector< double > timePred = (*data)[ *predictionRows ][ 0 ];
    //vector< double > timeLib  = (*data)[ *libraryRows    ][ 0 ];
    //timeIntersection = np.intersect1d( timePred, timeLib )
    //if len( timeIntersection ) :
    //    if args.verbose or args.warnings:
    //      print( 'FindNeighbors(): Degenerate library and prediction data.' )
        
    Neighbors neighbors; // { IndexBlock neighbors; DataBlock distances; }

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

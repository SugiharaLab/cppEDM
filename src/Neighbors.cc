
#include <algorithm>
#include <iterator>

#include "Neighbors.h"

//----------------------------------------------------------------
Neighbors::Neighbors() {
    neighbors = NULL;
    distances = NULL;
}
Neighbors::~Neighbors() {
    if ( neighbors ) { delete neighbors; }
    if ( distances ) { delete distances; }
}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
struct Neighbors *FindNeighbors(
    Matrix<double>   *matrix,
    std::vector<int> *libraryRows,
    std::vector<int> *predictionRows,
    Parameters       *parameters )
{

    int N_library_rows    = libraryRows->size();
    int N_prediction_rows = predictionRows->size();
    int N_columns         = matrix->NColumns();
    
#ifdef DEBUG
    std::cout << "FindNeighbors(): library:" << std::endl;
    for ( size_t row = 0; row < N_library_rows; row++ ) {
        int row_i = (*libraryRows)[row];
        std::cout << "row " << row_i << " : ";
        for ( size_t col = 0; col < N_columns; col++ ) {
            std::cout << (*matrix)(row_i,col) << " "; 
        } std::cout << std::endl;
    }
    std::cout << "FindNeighbors(): prediction:" << std::endl;
    for ( size_t row = 0; row < N_prediction_rows; row++ ) {
        int row_i = (*predictionRows)[row];
        std::cout << "row " << row_i << " : ";
        for ( size_t col = 0; col < N_columns; col++ ) {
            std::cout << (*matrix)(row_i,col) << " "; 
        } std::cout << std::endl;
    }
#endif

    std::valarray< double > time = matrix->column(0);

#ifdef JP_REMOVE //----------------------------------------
    std::cout << "Neighbors(): time: ";
    for ( size_t i = 0; i < time.size(); i++ ) {
        cout << time[i] << " ";
    } std::cout << endl;
#endif // JP REMOVE ---------------------------------------

    std::valarray< double > predTime( predictionRows->size() );
    std::valarray< double > libTime ( libraryRows->size()    );

    for ( size_t i = 0; i < predictionRows->size(); i++ ) {
        predTime[ i ] = time[ (*predictionRows)[i] ];
    }
    for ( size_t i = 0; i < libraryRows->size(); i++ ) {
        libTime[ i ] = time[ (*libraryRows)[i] ];
    }
   
#ifdef JP_REMOVE //----------------------------------------
    std::cout << "FindNeighbors(): libTime:  ";
    for ( size_t i = 0; i < libTime.size(); i++ ) {
        cout << libTime[i] << " ";
    } std::cout << std::endl;
    std::cout << "FindNeighbors(): predTime: ";
    for ( size_t i = 0; i < predTime.size(); i++ ) {
        std::cout << predTime[i] << " ";
    } std::cout << std::endl;
#endif // JP REMOVE ----------------------------------------

    // Identify degenerate library : prediction points by time comparison
    // set_intersection() needs a result vector
    std::vector< double > result( N_library_rows + N_prediction_rows, 0 );
    std::vector< double >::iterator ii;
    
    ii = set_intersection( begin( predTime ), end( predTime ),
                           begin( libTime  ), end( libTime  ), 
                           result.begin() );

    if ( ii != result.begin() ) {
        // Overlapping times exist
        std::stringstream msg;
        msg << "WARNING: FindNeighbors(): Degenerate library and prediction "
            << "data found. Overlap times: ";
        for ( auto ri = result.begin(); ri != ii; ++ri ) {
            msg << *ri << " ";
        } msg << std::endl;
        std::cout << msg.str();
    }

    // Instantiate a Neighbors struct
    // JP: I'm conflicted on whether to instantiate here and return
    // this object, or have the caller pass in the object with pointers
    // to the neighbors and distances matrices, or to create an overall
    // model object to contain this.
    Neighbors *neighbors = new Neighbors;
    neighbors->neighbors = new Matrix<int> ( N_prediction_rows,
                                             parameters->knn );
    neighbors->distances = new Matrix<double> ( N_prediction_rows,
                                                parameters->knn );

    // Vectors to hold the indices and values from each comparison
    std::valarray<int>    k_NN_neighbors( parameters->knn );
    std::valarray<double> k_NN_distances( parameters->knn );

    //-------------------------------------------------------------------
    // For each prediction vector (row in prediction Matrix) find the list
    // of library indices that are within k_NN points
    //-------------------------------------------------------------------
    for ( size_t row_i = 0; row_i < predictionRows->size(); row_i++ ) {
        int pred_row = (*predictionRows)[ row_i ];
        std::valarray<double> pred_row_vals = matrix->row( pred_row );
        
        // Exclude the 1st column (j=0) of times
        // JP Does N_columns need to be limited to E+1?
        std::valarray<double> y = pred_row_vals[std::slice(1, N_columns-1, 1)];

#ifdef JP_REMOVE //----------------------------------------
        std::cout << "Predict row " << pred_row << " : " ;
        std::cout << "predTime " << time[ pred_row ] << " | ";
        for ( size_t i = 0; i < y.size(); i++ ) {
            std::cout << y[i] << " ";
        } std::cout << std::endl;
#endif // JP REMOVE ----------------------------------------

        // Reset the neighbor and distance vectors
        for ( size_t i = 0; i < parameters->knn; i++ ) {
            k_NN_neighbors[ i ] = 0;
            // JP: This is dumb... don't use a hardcoded threshold.
            k_NN_distances[ i ] = 1E30;
        }

        //--------------------------------------------------------------
        // Library Rows
        //--------------------------------------------------------------
        for ( size_t row_j = 0; row_j < libraryRows->size(); row_j++ ) {
            int lib_row = (*libraryRows)[ row_j ];
            std::valarray<double> lib_row_vals = matrix->row( lib_row );
            std::valarray<double> x = lib_row_vals[std::slice(1,N_columns-1,1)];

#ifdef JP_REMOVE //----------------------------------------
            std::cout << "Library row " << lib_row << " : " ;
            std::cout << "libTime " << time[ lib_row ] << " | ";
            for ( size_t k = 0; k < lib_row_vals.size(); k++ ) {
                std::cout << lib_row_vals[k] << " ";
            } std::cout << std::endl;
#endif // JP REMOVE ----------------------------------------
            
            // If the library point is degenerate with the prediction,
            // ignore it. This is detected by comparing the timestamp. 
            if ( time [ lib_row ] == time[ pred_row ] ) {
                if ( parameters->verbose ) {
                    std::stringstream msg;
                    msg << "FindNeighbors(): Ignoring degenerate lib_row "
                        << lib_row << " time: " << time[ lib_row ]
                        << " and pred_row " << pred_row << " time: "
                        << time[ pred_row ] << std::endl;
                    std::cout << msg.str();
                }
                continue;
            }

            // If this lib_row + args.Tp >= library_N_row, then this neighbor
            // would be outside the library, keep looking if noNeighborLimit
            if ( lib_row + parameters->Tp >= N_library_rows ) {
                if ( not parameters->noNeighborLimit ) {
                    continue;
                }
            }
            
            // Find distance between the prediction vector (y)
            // and each of the library vectors (x)
            // Exclude the 1st column (j=0) of Time
            double d_i = Distance( &x, &y, DistanceMetric::Euclidean );

            // If d_i is less than values in k_NN_distances, add to list
            auto max_it = std::max_element( begin( k_NN_distances ),
                                            end( k_NN_distances ) );
            if ( d_i < *max_it ) {
                size_t max_i = std::distance( begin(k_NN_distances),
                                                   max_it );
                k_NN_neighbors[ max_i ] = lib_row;  // Save the index
                k_NN_distances[ max_i ] = d_i;      // Save the value
            }
        } // for ( row_j = 0; row_j < libraryRows->size(); row_j++ )
        
        if ( *std::max_element( begin( k_NN_distances ),
                                end  ( k_NN_distances ) ) > 1E29 ) {
            std::stringstream errMsg;
            errMsg << "FindNeighbors(): Library is too small to resolve "
                   << parameters->knn << " knn neighbors.";
            throw std::runtime_error( errMsg.str() );
        }
            
        // Check for ties.  JP: Need to address this, not just warning
        // First sort a copy of k_NN_neighbors so unique() will work
        std::valarray<int> k_NN_neighborCopy( k_NN_neighbors );
        std::sort( begin( k_NN_neighborCopy ), end( k_NN_neighborCopy ) );
        
        // ui is iterator to first non unique element
        auto ui = std::unique( begin(k_NN_neighborCopy),
                               end  (k_NN_neighborCopy) );
        
        if ( std::distance( begin( k_NN_neighborCopy ), ui ) !=
             k_NN_neighborCopy.size() ) {
            std::cout << "WARNING: FindNeighbors(): Degenerate neighbors."
                      << std::endl;
        }

        // Write the neighbor indices and distance values
        neighbors->neighbors->writeRow( row_i, k_NN_neighbors );
        neighbors->distances->writeRow( row_i, k_NN_distances );
        
    } // for ( row_i = 0; row_i < predictionRows->size(); row_i++ )

#ifdef DEBUG
    std::cout << "FindNeighbors(): neighbors:distances" << std::endl;
    for ( size_t i = 0; i < 3; i++ ) {
        std::cout << "Row " << i << " | ";
        for ( size_t j = 0; j < neighbors->neighbors->NColumns(); j++ ) {
            std::cout << (*neighbors->neighbors)( i, j ) << " : ";
            std::cout << (*neighbors->distances)( i, j ) << ",  ";
        } std::cout << std::endl;
    }
#endif
    return neighbors;
}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
double Distance( std::valarray<double> *v1,
                 std::valarray<double> *v2,
                 DistanceMetric         metric )
{
    double distance = 0;

    // For efficiency sake, we forego the usual validation of v1 & v2.

    if ( metric == DistanceMetric::Euclidean ) {
        // [sum_{i,j} abs(a_{i,j})^2]^{1/2}
        double sum = 0;
        for ( size_t i = 0; i < v1->size(); i++ ) {
            sum += pow( abs( (*v2)[i] - (*v1)[i] ), 2 );
        }
        distance = sqrt( sum );
    }
    else if ( metric == DistanceMetric::Manhattan ) {
        // max( sum( abs(x) ) )
        double sum = 0;
        for ( size_t i = 0; i < v1->size(); i++ ) {
            sum += abs( (*v2)[i] - (*v1)[i] );
        }
        distance = sum;
    }
    else {
        std::stringstream errMsg;
        errMsg << "Distance() Invalid DistanceMetric: "
               << static_cast<int>( metric );
        throw std::runtime_error( errMsg.str() );
    }

    return distance;
}

// Contains class definitions for EDM object. See EDM.h header for 
// description of EDM class and how to use the methods.

#include "EDM.h"


namespace EDM_Neighbors {
    // Define the initial maximum distance for neigbors to avoid sort()
    // DBL_MAX is a Macro equivalent to: std::numeric_limits<double>::max()
    double DistanceMax   = std::numeric_limits<double>::max();
    double DistanceLimit = std::numeric_limits<double>::max() - 1;
}

//----------------------------------------------------------------
// Distance computation between two vectors for several metrics
//----------------------------------------------------------------
double Distance( const std::valarray<double> &v1,
                 const std::valarray<double> &v2,
                 DistanceMetric metric )
{
    double distance = 0;

    // For efficiency sake, we forego the usual validation of v1 & v2.

    if ( metric == DistanceMetric::Euclidean ) {
        double sum   = 0;
        double delta = 0;
        for ( size_t i = 0; i < v1.size(); i++ ) {
            delta = v2[i] - v1[i];
            sum  += delta * delta; // avoid call to pow()
        }
        distance = sqrt( sum );

        // Note: this implicit implementation is slower
        // std::valarray<double> delta = v2 - v1;
        // distance = sqrt( (delta * delta).sum() );
    }
    else if ( metric == DistanceMetric::Manhattan ) {
        double sum = 0;
        for ( size_t i = 0; i < v1.size(); i++ ) {
            sum += abs( v2[i] - v1[i] );
        }
        distance = sum;
    }
    else {
        std::stringstream errMsg;
        errMsg << "Distance() Invalid DistanceMetric: "
               << static_cast<size_t>( metric );
        throw std::runtime_error( errMsg.str() );
    }

    return distance;
}

//----------------------------------------------------------------
// EDM()      : Constructor
//              Creates the data embedding and checks parameters
//
// data       : Input dataframe containing the time series to model
// method     : Enum on which method is using this EDM object (for parameters)
// E          : The embedding dimension to use when performing embedding
// tau        : The steps between each index in the embedding. Negative
//              tau is probably what you intend to use; positive tau yields
//              an embedding (E_t,E_t+tau...) (future forward embedding)
// columns    : Columns for embedding
// targetName : Dimension to project onto for prediction
// embedded   : Whether data supplied is already embedded or not
// verbose    : Verbose information flag
// 
//----------------------------------------------------------------
EDM::EDM ( DataFrame<double> & data,  int E, int tau, 
        std::string columns, std::string targetName, 
        bool embedded, bool verbose  ): data(data), targetName( targetName ){

    // Check parameters

    // Create embedding 

    embedding = embedded ? data : Embed( data, E, tau, columns, verbose );
    
}

//----------------------------------------------------------------
// EDM()      : ComputeNeighbors
//              Computes neighbors for every prediction index
// lib, pred  : Library and prediction ranges
// verbose    : Verbose information flag
//
// return     : List of DF where first element is neighbors, second is distances
// 
//----------------------------------------------------------------
EDM::Neighbors EDM::ComputeNeighbors ( std::string lib, std::string pred, 
        int Tp, int knn, int exclusionRadius, bool verbose  ){
    
    ////////////////////////////////////////////////////
    // Parse prediction and lib into vectors of indices 
    ////////////////////////////////////////////////////

    // Holds the vector of indices for both lib and pred
    std::vector< std::vector< size_t > > ranges;

    // Process both range strings
    for ( std::string rangeStr : {lib, pred} ) {

        // Validate that number of ranges is even  
        std::vector<std::string> rangeVec = SplitString( rangeStr, " \t," );
        if ( rangeVec.size() % 2 != 0 ) {
            std::string errMsg( "Parameters::Validate(): "
                        "disjoint range must be even number of integers.\n" );
            throw std::runtime_error( errMsg );
        }

        // Generate vector of start, stop index pairs
        std::vector< std::pair< size_t, size_t > > rangePairs;
        for ( size_t i = 0; i < rangeVec.size(); i = i + 2 ) {
            rangePairs.emplace_back( std::make_pair(std::stoi( rangeVec[i]),
                                                    std::stoi( rangeVec[i+1])));
        }

        // Create library vector of indices

        std::vector<size_t> rangeIndicesVec;

        for ( auto thisPair : rangePairs ) {
            for ( size_t li = thisPair.first; li <= thisPair.second; li++ ) {

                rangeIndicesVec.push_back( li - 1 ); // apply zero-offset

            }
        }
        ranges.push_back( rangeIndicesVec );
    }

    std::vector<size_t> libraryIndices = ranges[0];
    std::vector<size_t> predIndices    = ranges[1];

    auto max_lib_it      = std::max_element(libraryIndices.begin(), 
                                              libraryIndices.end() );
    size_t max_lib_index = *max_lib_it;
    
    // Check/Set knn. Note that SMap should set knn to -1 for full library
    // If knn=-1, set to full library, if knn=0, set to E+1, if E>knn>-1, error

    if ( knn == -1 ) {
        knn = libraryIndices.size() - Tp * (data.NColumns() + 1);
    }
    else if ( knn == 0 ) {

        knn = data.NColumns() + 1;
        
        if ( verbose ) {
            std::stringstream msg;
            msg << "Parameters::Validate(): Set knn = " << knn
                << "  " << std::endl;
            std::cout << msg.str();
        }
    }
    else if ( knn < data.NColumns() + 1 ) {
        std::stringstream errMsg;
        errMsg << "Parameters::Validate(): knn of " << knn
               << " is less than E+1 = " << data.NColumns() + 1 << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    // Neighbors: struct on local stack to be returned by copy
    Neighbors neighbors;
    neighbors.neighbors = DataFrame<size_t> (predIndices.size(), knn);
    neighbors.distances = DataFrame<double> (predIndices.size(), knn);

    // Vectors to hold indices and values from each comparison
    std::valarray<size_t> k_NN_neighbors( knn );
    std::valarray<double> k_NN_distances( knn );

    //-------------------------------------------------------------------
    // For each prediction vector (row in prediction DataFrame) find the
    // list of library indices that are within k_NN points
    //-------------------------------------------------------------------
    for ( size_t pred_row_idx = 0; pred_row_idx < predIndices.size(); pred_row_idx++ ) {

        // Get the current query/pred row
        size_t pred_row = predIndices[ pred_row_idx ];
        std::valarray<double> pred_vec = data.Row( pred_row );
        
        // Reset the neighbor and distance vectors for this pred row
        for ( size_t i = 0; i < knn; i++ ) {
            k_NN_neighbors[ i ] = 0;
            // JP: Used to avoid sort()
            k_NN_distances[ i ] = EDM_Neighbors::DistanceMax;
        }

        //--------------------------------------------------------------
        // Compute distance on every library row to query/pred row
        //--------------------------------------------------------------
        for ( size_t row_j = 0; row_j < libraryIndices.size(); row_j++ ) {
            // Get the library vector for this lib_row index
            size_t lib_row = libraryIndices[ row_j ];
            std::valarray<double> lib_vec = data.Row( lib_row );
            
            // If the library point is degenerate with the prediction,
            // ignore it.
            if ( lib_row == pred_row ) {
#ifdef DEBUG_ALL
                if ( verbose ) {
                    std::stringstream msg;
                    msg << "FindNeighbors(): Ignoring degenerate lib_row "
                        << lib_row << " and pred_row " << pred_row << std::endl;
                    std::cout << msg.str();
                }
#endif
                continue;
            }

            // Apply temporal exclusion radius: units are data rows, not time
            if ( exclusionRadius ) {
                int xrad = (int) lib_row - pred_row;
                if ( std::abs( xrad ) <= exclusionRadius ) {
                    continue;
                }
            }
            // If this lib_row + args.Tp >= max_lib_index, then this neighbor
            // would be outside the library, keep looking if noNeighborLimit
            if ( not noNeighborLimit ) {
                if ( lib_row + Tp > max_lib_index ) {
                    continue;
                }
            }
            
            // Find distance between the prediction vector
            // and each of the library vectors
            // The 1st column (j=0) of Time has been excluded above
            double d_i = Distance( lib_vec, pred_vec,
                                   DistanceMetric::Euclidean );

            // If d_i is less than values in k_NN_distances, add to list
            auto max_it = std::max_element( begin( k_NN_distances ),
                                            end( k_NN_distances ) );
            if ( d_i < *max_it ) {
                size_t max_i = std::distance( begin(k_NN_distances), max_it );
                k_NN_neighbors[ max_i ] = lib_row;  // Save the index
                k_NN_distances[ max_i ] = d_i;      // Save the value
            }
        } // for ( row_j = 0; row_j < library.size(); row_j++ )

        // Assert that at least knn were found 
        if ( *std::max_element( begin( k_NN_distances ),
                                end  ( k_NN_distances ) ) >
             EDM_Neighbors::DistanceLimit ) {
            
            std::stringstream errMsg;
            errMsg << "FindNeighbors(): Failed to find "
                   << knn << " knn neighbors. The library "
                   << "may be too small." << std::endl;
            throw std::runtime_error( errMsg.str() );
        }

        // Check for ties.  JP: Need to address this, not just warning
        // First sort a copy of k_NN_neighbors so unique() will work
        std::valarray<size_t> k_NN_neighborCopy( k_NN_neighbors );
        std::sort( begin( k_NN_neighborCopy ), end( k_NN_neighborCopy ) );
        
        // ui is iterator to first non unique element
        auto ui = std::unique( begin( k_NN_neighborCopy ),
                               end  ( k_NN_neighborCopy ) );
        
        if ( std::distance( begin( k_NN_neighborCopy ), ui ) !=
             k_NN_neighborCopy.size() ) {
            std::cout << "WARNING: FindNeighbors(): Degenerate neighbors./n";
        }

        // Write the neighbor indices and distance values
        neighbors.neighbors.WriteRow( pred_row_idx, k_NN_neighbors );
        neighbors.distances.WriteRow( pred_row_idx, k_NN_distances );

    } // for ( pred_row_idx = 0; pred_row_idx < predictionRows->size(); pred_row_idx++ )

    return neighbors;

}

//----------------------------------------------------------------
// EDM()    : Project
//            Finds neighbors, performs weighting on neighbors, and
//            projects onto pred range.
//
// data     : Input dataframe containing the time series to model.
//            tau is probably what you intend to use; positive tau yields
//            an embedding (E_t,E_t+tau...) (future forward embedding).
//----------------------------------------------------------------
std::list< DataFrame<double> > EDM::Project ( std::string lib, std::string pred, 
                          int Tp, int knn, int exclusionRadius, bool verbose ) 
{

    // Find neighbors
    ComputeNeighbors(lib,pred,Tp,knn,exclusionRadius,verbose);

    // Weight neighbors

    // Project onto target for prediction

    return std::list<DataFrame<double>>();
}


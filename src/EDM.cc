// Contains class definitions for EDM object. See EDM.h header for 
// description of EDM class and how to use the methods.

#include "EDM.h"
#include "EDM_Helpers.cc"

namespace EDM_Neighbors {
    // Define the initial maximum distance for neigbors to avoid sort()
    // DBL_MAX is a Macro equivalent to: std::numeric_limits<double>::max()
    double DistanceMax   = std::numeric_limits<double>::max();
    double DistanceLimit = std::numeric_limits<double>::max() - 1;
}

#ifdef DEBUG_ALL
void PrintNeighbors( const EDM::Neighbors &neighbors );
#endif

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
// embedded   : Whether data supplied is already embedded or not
// verbose    : Verbose information flag
// 
//----------------------------------------------------------------
EDM::EDM ( DataFrame<double> & data,
           int                 E,
           int                 tau,
           bool                embedded, 
           std::string         columns,
           std::string         target,
           bool                noNeighborLimit,
           bool                verbose ): 
    data(data), E(E), tau(tau), embedded(embedded),
    columns(columns), target(target), noNeighborLimit(noNeighborLimit),
    verbose(verbose) {

    //-------------------------------------------------------------
    // Validate parameters and create embedding
    //-------------------------------------------------------------

    if ( not embedded and tau == 0 ) {
        std::string errMsg( "Parameters::Validate(): "
                            "tau must be non-zero.\n" );
        throw std::runtime_error( errMsg );
    }

    std::vector<std::string> columnNames = ParseColumnNames( columns );
    std::vector<size_t>      columnIndex = ParseColumnIndices( columns );

    if ( not columnIndex.size() and not columnNames.size() ) {
        std::stringstream errMsg;
        errMsg << "Parameters::Validate(): No valid columns found." << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    if ( embedded ) {
        // dataIn is a multivariable block, no embedding needed
        // Select the specified columns into dataBlock
        embedding = columnNames.size() ? 
                    data.DataFrameFromColumnNames( columnNames ) : 
                    data.DataFrameFromColumnIndex( columnIndex );
    }
    else {
        // embedded = false: Create the embedding dataBlock via Embed()
        // dataBlock will have tau * (E-1) fewer rows than dataIn
        embedding = Embed( data, E, tau, columns, verbose );
    }
}

//----------------------------------------------------------------
// EDM()      : Project
//              Finds neighbors, performs weighting on neighbors, and
//              projects onto pred range.
//
// target     : Dimension to project onto for prediction
//
// data       : Input dataframe containing the time series to model.
//              -tau is probably what you intend to use; positive tau yields
//              an embedding (E_t,E_t+tau...) (future forward embedding).
//----------------------------------------------------------------
std::list< DataFrame<double> > EDM::Project ( std::string lib,
                                              std::string pred, 
                                              std::string target,
                                              int         Tp,
                                              int         knn, 
                                              int         exclusionRadius,
                                              bool        verbose ) {

    // Parse lib and pred range strings
    std::vector<size_t> libIndices  = ParseRangeStr( lib );
    std::vector<size_t> predIndices = ParseRangeStr( pred );
    
    CheckDataRows( data.NRows(), libIndices.back(), predIndices.back(),
                   E, tau, embedded );

    //----------------------------------------------------------
    // Get target (library) vector
    //----------------------------------------------------------

    std::valarray<double> targetIn;

    std::vector< size_t > colIndices    = ParseColumnIndices(target);
    std::vector< std::string > colNames = ParseColumnNames(target);

    // Note CS : check with JP to make sure target range selection appropriate
    // If a column name or idx specified, extract that vector
    if ( colIndices.size() or colNames.size() ) {

        // Get name of target column in embedding 

        std::string targName;  

        // If embedded, then column names are not formatted 
        if (embedded) {
            targName = colNames.size()? colNames[0] : 
                                        embedding.ColumnNames()[ colIndices[0] ];
        }
        // If we performed embedding, reproduce formatting of target col name 
        // as prefix "V" if idx specified, and append (t+-0)
        else {
            std::string originalName = colNames.size() ? colNames[0] : 
                                          "V" + std::to_string( colIndices[0] );
            char tauSign = tau > 0 ? '+' : '-';
            targName = originalName+ "(t" + tauSign + "0)";
        }

        targetIn = embedding.VectorColumnName( targName );
    }
    // Default to first column
    else {
        targetIn = data.Column( 0 );
    }

    // Shift lib and pred by number of partial rows if we embedded 

    if ( not embedded ) {

        size_t shift = abs( tau ) * ( E - 1 );

        if ( shift > 0 ) {
            libIndices  = AdjustRange( shift, tau, libIndices);
            predIndices = AdjustRange( shift, tau, predIndices);
        }

        // Check boundaries again since rows were potentially removed
        CheckDataRows( data.NRows(), libIndices.back(), predIndices.back(),
                   E, tau, embedded );
    }

    // Find neighbors
    FindNeighbors( libIndices, predIndices, Tp, knn, exclusionRadius, verbose );

    // Weight neighbors

    // Project onto target for prediction

    return std::list<DataFrame<double>>();
}

//----------------------------------------------------------------
// EDM()      : FindNeighbors
//              Computes neighbors for every prediction index
// lib, pred  : Library and prediction ranges
// verbose    : Verbose information flag
//
// return     : List of DF where first element is neighbors, second is distances
// 
//----------------------------------------------------------------
EDM::Neighbors EDM::FindNeighbors ( std::vector<size_t> libraryIndices,
                                    std::vector<size_t> predIndices,
                                    int                 Tp,
                                    int                 knn,
                                    int                 exclusionRadius,
                                    bool                verbose  ) {
    
    //-------------------------------------------------------------------------
    // Check/Set knn. Note that SMap should set knn to -1 for full library
    // If knn=-1, set to full library, if knn=0, set to E+1, if E>knn>-1, error
    //-------------------------------------------------------------------------

    if ( knn == -1 ) {
        knn = libraryIndices.size() - Tp * (E + 1);
    }
    else if ( knn == 0 ) {

        knn = E + 1;
        
        if ( verbose ) {
            std::stringstream msg;
            msg << "Parameters::Validate(): Set knn = " << knn
                << "  " << std::endl;
            std::cout << msg.str();
        }
    }
    else if ( knn < E + 1 ) {
        std::stringstream errMsg;
        errMsg << "Parameters::Validate(): knn of " << knn
               << " is less than E+1 = " << E + 1 << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    //-------------------------------------------------------------------------
    // Variables for holding neighbor results and definitions for convenience 
    //-------------------------------------------------------------------------

    // Neighbors: struct on local stack to be returned by copy
    Neighbors neighbors;
    neighbors.neighbors = DataFrame<size_t> (predIndices.size(), knn);
    neighbors.distances = DataFrame<double> (predIndices.size(), knn);

    // Vectors to hold indices and values from each comparison
    std::valarray<size_t> k_NN_neighbors( knn );
    std::valarray<double> k_NN_distances( knn );

    // Max index for out of bounds query in exclusion function 
    auto max_lib_it      = std::max_element(libraryIndices.begin(), 
                                                          libraryIndices.end() );
    size_t max_lib_index = *max_lib_it;

    //-------------------------------------------------------------------
    // For each prediction vector (row in prediction DataFrame) find the
    // list of library indices that are within k_NN points
    //-------------------------------------------------------------------
    for ( size_t pred_row_idx = 0; pred_row_idx < predIndices.size();
          pred_row_idx++ ) {


        // Get the current query/pred row
        size_t pred_row = predIndices[ pred_row_idx ];
        std::valarray<double> pred_vec = embedding.Row( pred_row );
        
        // Reset the neighbor and distance vectors for this pred row
        for ( int i = 0; i < knn; i++ ) {
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
            std::valarray<double> lib_vec = embedding.Row( lib_row );
            
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
             (long int) k_NN_neighborCopy.size() ) {
            std::cout << "WARNING: FindNeighbors(): Degenerate neighbors./n";
        }

        // Write the neighbor indices and distance values
        neighbors.neighbors.WriteRow( pred_row_idx, k_NN_neighbors );
        neighbors.distances.WriteRow( pred_row_idx, k_NN_distances );

    } // for ( pred_row_idx = 0; pred_row_idx < predictionRows->size(); pred_row_idx++ )

    #ifdef DEBUG_ALL
    const Neighbors &neigh = neighbors;
    PrintNeighbors( neigh );
    #endif

    return neighbors;

}


//----------------------------------------------------------------
// Debug method to print out all neighbors
//----------------------------------------------------------------
#ifdef DEBUG_ALL
void PrintNeighbors( const EDM::Neighbors &neighbors )
{
    std::cout << "FindNeighbors(): neighbors:distances" << std::endl;
    //for ( size_t i = 0; i < neighbors.neighbors.NRows(); i++ ) {
    for ( size_t i = 0; i < 5; i++ ) {
        std::cout << "Row " << i << " | ";
        for ( size_t j = 0; j < neighbors.neighbors.NColumns(); j++ ) {
            std::cout << neighbors.neighbors( i, j ) << " ";
        } std::cout << "   : ";
        for ( size_t j = 0; j < neighbors.neighbors.NColumns(); j++ ) {
            std::cout << neighbors.distances( i, j ) << " ";
        } std::cout << std::endl;
    }
}
#endif

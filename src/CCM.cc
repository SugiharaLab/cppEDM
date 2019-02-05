
#include <cstdlib>

#include "Parameter.h"
#include "DataFrame.h"
#include "Embed.h"
#include "Neighbors.h"

// forward declaration
std::valarray< double > CrossMap( Parameters p, DataFrame< double > df );
DataFrame< double > CCMDistances( DataFrame< double > dataBlock,
                                  Parameters param );

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
CCMResult CCM( std::string pathIn,
               std::string dataFile,
               std::string pathOut,
               std::string predictFile,
               std::string lib,
               std::string pred,
               int         E,
               int         Tp,
               int         knn,
               int         tau,
               std::string columns,
               std::string target,
               std::string libSizes_str,
               int         sample,
               bool        random,
               int         seed,
               bool        embedded,
               bool        verbose )
{
    if ( not columns.size() ) {
        throw std::runtime_error("CCM() must specify the column to embed.");
    }
    if ( not target.size() ) {
        throw std::runtime_error("CCM() must specify the target.");
    }

    Parameters param = Parameters( Method::Simplex, pathIn, dataFile,
                                   pathOut, predictFile,
                                   lib, pred, E, Tp, knn, tau, 0,
                                   columns, target, embedded, verbose,
                                   "", "", "", 0, 0, 0, 0,
                                   libSizes_str, sample, random, seed );

    if ( param.columnNames.size() > 1 ) {
        std::cout << "WARNING: CCM() Only the first column will be mapped.\n";
    }

    // Create a local Parameters object that switches column[0] and
    // target for the inverse mapping
    Parameters inverseParam = param; // copy constructor
    std::string newTarget( param.columnNames[0] );
    inverseParam.columnNames[0] = param.targetName;
    inverseParam.targetName     = newTarget;
    
#ifdef DEBUG
    std::cout << "CCM() params:\n";
    std::cout << param;
    std::cout << "CCM() inverseParams:\n";
    std::cout << inverseParam;
#endif

    // If seed is set to 1, the generator is reinitialized to its
    // initial value and produces the same values as before any call
    // to rand or srand.
    if ( seed >= 0 ) { srand( seed ); }

    //----------------------------------------------------------
    // Load data to dataFrameIn
    //----------------------------------------------------------
    DataFrame< double > dataFrameIn( param.pathIn, param.dataFile );

    // Put in array or pair to iteratate loop for OpenMP....
    std::valarray< double > col_to_target = CrossMap( param, dataFrameIn );

    std::valarray< double > target_to_col = CrossMap(inverseParam,dataFrameIn);


    
    
    CCMResult result = CCMResult();

    return result;
}


//----------------------------------------------------------------
// Return vector of rho values for param.librarySizes
//----------------------------------------------------------------
std::valarray< double > CrossMap( Parameters param,
                                  DataFrame< double > dataFrameIn ) {

    //----------------------------------------------------------
    // Generate embedding on the data to be cross mapped (-c column)
    //----------------------------------------------------------
    DataFrame<double> dataBlock; // Multivariate or embedded DataFrame

    dataBlock = Embed( dataFrameIn, param.E, param.tau,
                       param.columnNames[0], param.verbose );

    size_t N_row = dataBlock.NRows();
    
    //----------------------------------------------------------
    // Get target (library) vector
    // Require the target to be specified by column name, not index
    //----------------------------------------------------------
    std::valarray<double> target_vec;
    if ( param.targetIndex ) {
        std::runtime_error("CrossMap() target must be specified by column name");
    }
    else {
        target_vec = dataFrameIn.VectorColumnName( param.targetName );
    }

    size_t maxSamples;
    if ( param.randomLib ) {
        // Random samples from library with replacement
        maxSamples = param.subSamples;
    }
    else {
        // Contiguous samples up to the size of the library
        maxSamples = param.librarySizes[ 2 ];
    }

    if ( param.verbose ) {
        std::stringstream msg;
        msg << "CCM(): Simplex cross mapping from " << param.columnNames[0]
            << " to " + param.targetName << "  E=" << param.E
            << "  knn=" << param.knn << "  Library range: ["
            << param.librarySizes[ 0 ] << " " << param.librarySizes[ 1 ]
            << " " << param.librarySizes[ 2 ] << "]\n";
        std::cout << msg.str();
    }
    
    //-----------------------------------------------------------------
    // Distance for all possible pred : lib E-dimensional vector pairs
    // Distances is a Matrix of all row to to row distances
    //-----------------------------------------------------------------
    DataFrame< double >  Distances = CCMDistances( dataBlock, param );


    //-----------------------------------------------------------------
    // Output
    //-----------------------------------------------------------------
    std::valarray< double > rho( param.librarySizes.size() );
    
    
    return rho;
}

//--------------------------------------------------------------------- 
// Note that for CCM the library and prediction rows are the same.
//
// Return Distances: a square matrix with distances.
// Matrix elements D[i,j] hold the distance between the E-dimensional
// phase space point (vector) between rows (observations) i and j.
//---------------------------------------------------------------------
DataFrame< double > CCMDistances( DataFrame< double > dataBlock,
                                  Parameters param ) {
    
    size_t N_row = dataBlock.NRows();

    size_t E = param.E;

    DataFrame< double > D = DataFrame< double >( N_row, N_row );

    for ( size_t row = 0; row < N_row; row++ ) {
        // Get E-dimensional vector from this library row
        std::valarray< double > v1_ = dataBlock.Row( row );
        // Exclude the 1st column (j=0) of times
        // JP should this be E + 1 ?
        std::valarray< double > v1 = v1_[ std::slice( 1, E, 1 ) ];

        for ( size_t col = 0; col < N_row; col++ ) {
            // Ignore the diagonal (row == col)
            if ( row == col ) {
                continue;
            }
            
            // Find distance between vector (v) and other library vector
            std::valarray< double > v2_ = dataBlock.Row( col );
            // Exclude the 1st column (j=0) of times
            // JP should this be E + 1 ?
            std::valarray< double > v2 = v2_[ std::slice( 1, E, 1 ) ];
            
            D( row, col ) = Distance( v1, v2, DistanceMetric::Euclidean );
            
            // Insert degenerate values since D[i,j] = D[j,i]
            D( col, row ) = D( row, col );
        }
    }
    return D;
}

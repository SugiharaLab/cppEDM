
#include <thread>
#include <atomic>
#include <mutex>

#include "Common.h"
#include "AuxFunc.h"

namespace EDM_Multiview {
    // Thread Work Queue : Vector of int
    typedef std::vector< int > WorkQueue;
    
    // atomic counter for all threads
    std::atomic<std::size_t> eval_i(0); // initialize to 0
    std::atomic<std::size_t> pred_i(0); // initialize to 0
    std::mutex mtx;
}

// forward declarations
std::vector< std::vector< size_t > > Combination( int n, int k );

DataFrame<double> SimplexProjection( Parameters param, DataEmbedNN embedNN );

void EvalComboThread( EDM_Multiview::WorkQueue             &wq,
                      DataFrame< double >                  &data,
                      std::vector< std::vector< size_t > > &combos,
                      DataFrame< double >                  &combos_rho,
                      Parameters                           &param );

//----------------------------------------------------------------
// Multiview() : Evaluate Simplex rho vs. dimension E
// API Overload 1: Explicit data file path/name
//     Implemented as a wrapper to API Overload 2:
//----------------------------------------------------------------
DataFrame<double> Multiview( std::string pathIn,
                             std::string dataFile,
                             std::string pathOut,
                             std::string predictFile,
                             std::string lib,
                             std::string pred,
                             int         E,
                             int         Tp,
                             int         tau,
                             std::string columns,
                             std::string target,
                             bool        verbose,
                             unsigned    nThreads ) {

    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );
    
    DataFrame<double> result = Multiview( dataFrameIn,
                                          pathOut,
                                          predictFile,
                                          lib,
                                          pred,
                                          E,
                                          Tp,
                                          tau,
                                          columns,
                                          target,
                                          verbose,
                                          nThreads );
    return result;
}

//----------------------------------------------------------------
// Multiview()
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> Multiview( DataFrame< double > data,
                             std::string         pathOut,
                             std::string         predictFile,
                             std::string         lib,
                             std::string         pred,
                             int                 E,
                             int                 Tp,
                             int                 knn,
                             int                 tau,
                             std::string         columns,
                             std::string         target,
                             bool                verbose,
                             unsigned            nThreads ) {
    
    Parameters param = Parameters( Method::Simplex, "", "",
                                   pathOut, predictFile,
                                   lib, pred, E, Tp, knn, tau, 0,
                                   columns, target, true, verbose );
    
    if ( not param.columnNames.size() ) {
        throw std::runtime_error( "Multiview() requires column names." );
    }
    if ( not param.targetName.size() ) {
        throw std::runtime_error( "Multiview() requires target name." );
    }
    if ( param.E < 1 ) {
        throw std::runtime_error( "Multiview() E is required." );
    }
    // Ensure that params are validated so columnNames are populated
    if ( not param.validated ) {
        throw std::runtime_error( "Multiview() params not validated." );        
    }

    // Save param.predictOutputFile and reset so Simplex() does not write 
    std::string outputFile = param.predictOutputFile;
    param.predictOutputFile = "";
    
    // Combinations of possible embedding variables (columns), E at-a-time
    // Note that these combinations are not zero-offset, i.e.
    // Combination( 3, 2 ) = [(1, 2), (1, 3), (2, 3)]
    // Since data has time in column 0, these correspond to column indices
    size_t nVar = param.columnNames.size();
    std::vector< std::vector< size_t > > combos =
        Combination( nVar * param.E + 1, param.E );

    // Establish number of ensembles if not specified
    if ( not param.MultiviewEnsemble ) {
        // Ye & Sugihara suggest sqrt( m ) as the number of embeddings to avg
        param.MultiviewEnsemble = std::max(2, (int) std::sqrt(combos.size()));
        
        std::stringstream msg;
        msg << "Multiview() Set view sample size to "
            << param.MultiviewEnsemble;
        std::cout << msg.str();
    }

    //---------------------------------------------------------------
    // Evaluate variable combinations.
    // Note that this is done within the library itself (in-sample).
    //---------------------------------------------------------------
    // Save a copy of the specified prediction observation rows.
    std::vector<size_t> prediction = param.prediction;
    
    // Override the param.prediction for in-sample forecast skill evaluation
    param.prediction = param.library;

    // Create column names for the results DataFrame
    // One row for each combo, E columns (a combo), and rho
    std::stringstream header;
    for ( auto i = 1; i <= param.E; i++ ) {
        header << "Col_" << i << " ";
    }
    header << "rho";

    // Results Data Frame
    DataFrame<double> combos_rho( combos.size(), param.E + 1, header.str() );

    // Build work queue
    EDM_Multiview::WorkQueue wq( combos.size() );

    // Insert combos index into work queue
    for ( auto i = 0; i < combos.size(); i++ ) {
        wq[ i ] = i + 1;
    }

    unsigned maxThreads = std::thread::hardware_concurrency();
    if ( maxThreads < nThreads ) { nThreads = maxThreads; }
    
    // thread container
    std::vector< std::thread > threads;
    for ( unsigned i = 0; i < nThreads; ++i ) {
        threads.push_back( std::thread( EvalComboThread,
                                        std::ref( wq ),
                                        std::ref( data ),
                                        std::ref( combos ),
                                        std::ref( combos_rho ),
                                        std::ref( param ) ) );
    }
    
    // join threads
    for ( auto &thrd : threads ) {
        thrd.join();
    }
    
}

//----------------------------------------------------------------
// Worker thread 
//----------------------------------------------------------------
void EvalComboThread( EDM_Multiview::WorkQueue             &wq,
                      DataFrame< double >                  &data,
                      std::vector< std::vector< size_t > > &combos,
                      DataFrame< double >                  &combos_rho,
                      Parameters                           &param )
{
    // atomic_fetch_add(): Adds val to the contained value and returns
    // the value it had immediately before the operation.
    std::size_t combo_i =
        std::atomic_fetch_add( &EDM_Multiview::eval_i, std::size_t(1) );

    // Get the combo for this thread
    std::vector< size_t > combo = combos[ combo_i ];

    // Select combo columns from the data
    DataFrame<double> comboData = data.DataFrameFromColumnIndex( combo );
    
    // Embed data & compute neighbors
    DataEmbedNN embedNN = EmbedNN( comboData, param );

    // Evaluate combo prediction
    DataFrame<double> S = SimplexProjection( param, embedNN );
    
    VectorError ve = ComputeError( S.VectorColumnName( "Observations" ),
                                   S.VectorColumnName( "Predictions"  ) );

    std::valarray< double > combo_row( combo.size() + 1 );
    for ( auto i = 0; i < combo.size(); i++ ) {
        combo_row[ i ] = combo[ i ];
    }
    combo_row[ combo.size() ] = ve.rho;

    combos_rho.WriteRow( combo_i, combo_row );
}

//----------------------------------------------------------------
// Return combinations C(n,k) as a vector of vectors
//----------------------------------------------------------------
std::vector< std::vector< size_t > > Combination( int n, int k ) {

    std::vector< bool > v(n);
    for ( size_t i = 0; i < n; ++i ) {
        v[i] = ( i >= (n - k) );
    }

    std::vector< std::vector< size_t > > combos;
    
    do {
        std::vector< size_t > this_combo( k );
        size_t j = 0;
        for ( size_t i = 0; i < n; ++i ) {
            if ( v[i] ) {
                this_combo[ j ] = i + 1;
                j++;
            }
        }
        // insert this tuple in the combos vector
        combos.push_back( this_combo );
        
    } while ( std::next_permutation( v.begin(), v.end() ) );

    return combos;
}


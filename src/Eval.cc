
#include <thread>
#include <atomic>
#include <mutex>

#include "Common.h"

namespace EDM_Eval {
    // Thread Work Queue : Vector of int
    typedef std::vector< int > WorkQueue;
    
    // atomic counters for all threads
    std::atomic<std::size_t> tp_count_i   (0); // initialize to 0
    std::atomic<std::size_t> embed_count_i(0); // initialize to 0
    std::atomic<std::size_t> smap_count_i (0); // initialize to 0
    std::mutex mtx;
}

//----------------------------------------------------------------
// Forward declaration:
// Worker thread for EmbedDimension() and PredictInterval()
//----------------------------------------------------------------
void SimplexThread( EDM_Eval::WorkQueue &wq,
                    DataFrame< double > &data,
                    DataFrame< double > &E_rho,
                    std::string          lib,
                    std::string          pred,
                    int                  E,
                    int                  Tp,
                    int                  tau,
                    std::string          colNames,
                    std::string          targetName,
                    bool                 embedded,
                    bool                 verbose );

//----------------------------------------------------------------
// Forward declaration:
// Worker thread for PredictNonLinear()
//----------------------------------------------------------------
void SMapThread( EDM_Eval::WorkQueue   &wq,
                 DataFrame< double >   &data,
                 DataFrame< double >   &Theta_rho,
                 std::valarray<double>  ThetaValues,
                 std::string            lib,
                 std::string            pred,
                 int                    E,
                 int                    Tp,
                 int                    tau,
                 std::string            colNames,
                 std::string            targetName,
                 bool                   embedded,
                 bool                   verbose );

//----------------------------------------------------------------
// EmbedDimension() : Evaluate Simplex rho vs. dimension E
// API Overload 1: Explicit data file path/name
//     Implemented as a wrapper to API Overload 2:
//----------------------------------------------------------------
DataFrame<double> EmbedDimension( std::string pathIn,
                                  std::string dataFile,
                                  std::string pathOut,
                                  std::string predictFile,
                                  std::string lib,
                                  std::string pred,
                                  int         Tp,
                                  int         tau,
                                  std::string colNames,
                                  std::string targetName,
                                  bool        embedded,
                                  bool        verbose,
                                  unsigned    nThreads ) {

    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );
    
    DataFrame<double> E_rho = EmbedDimension( dataFrameIn,
                                              pathOut,
                                              predictFile,
                                              lib,
                                              pred,
                                              Tp,
                                              tau,
                                              colNames,
                                              targetName,
                                              embedded,
                                              verbose,
                                              nThreads );
    return E_rho;
}

//----------------------------------------------------------------
// EmbedDimension() : Evaluate Simplex rho vs. dimension E
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> EmbedDimension( DataFrame< double > data,
                                  std::string         pathOut,
                                  std::string         predictFile,
                                  std::string         lib,
                                  std::string         pred,
                                  int                 Tp,
                                  int                 tau,
                                  std::string         colNames,
                                  std::string         targetName,
                                  bool                embedded,
                                  bool                verbose,
                                  unsigned            nThreads ) {

    // Container for results
    DataFrame<double> E_rho( 10, 2, "E rho" );

    // build work queue
    EDM_Eval::WorkQueue wq( 10 );

    // Insert dimension values into work queue
    for ( auto i = 0; i < 10; i++ ) {
        wq[ i ] = i + 1;
    }

    unsigned maxThreads = std::thread::hardware_concurrency();
    if ( maxThreads < nThreads ) { nThreads = maxThreads; }
    if ( nThreads > 10 )         { nThreads = 10;         }
    
    // thread container
    std::vector< std::thread > threads;
    for ( unsigned i = 0; i < nThreads; ++i ) {
        threads.push_back( std::thread( SimplexThread,
                                        std::ref( wq ),
                                        std::ref( data ),
                                        std::ref( E_rho ),
                                        lib,
                                        pred,
                                        0,       // E must be 0
                                        Tp,
                                        tau,
                                        colNames,
                                        targetName,
                                        embedded,
                                        verbose ) );
    }
    
    // join threads
    for ( auto &thrd : threads ) {
        thrd.join();
    }
    
    // Reset counter in case other threads are spawned in the app
    std::atomic_store( &EDM_Eval::embed_count_i, std::size_t(0) );
    
    if ( predictFile.size() ) {
        E_rho.WriteData( pathOut, predictFile );
    }
    
    return E_rho;
}

//-----------------------------------------------------------------
// PredictInterval() : Evaluate Simplex rho vs. predict interval Tp
// API Overload 1: Explicit data file path/name
//     Implemented as a wrapper to API Overload 2: 
//-----------------------------------------------------------------
DataFrame<double> PredictInterval( std::string pathIn,
                                   std::string dataFile,
                                   std::string pathOut,
                                   std::string predictFile,
                                   std::string lib,
                                   std::string pred,
                                   int         E,
                                   int         tau,
                                   std::string colNames,
                                   std::string targetName,
                                   bool        embedded,
                                   bool        verbose,
                                   unsigned    nThreads ) {
    
    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );
    
    DataFrame<double> Tp_rho = PredictInterval( dataFrameIn,
                                                pathOut,
                                                predictFile,
                                                lib,
                                                pred,
                                                E,
                                                tau,
                                                colNames,
                                                targetName,
                                                embedded,
                                                verbose );
    return Tp_rho;
}

//-----------------------------------------------------------------
// PredictInterval() : Evaluate Simplex rho vs. predict interval Tp
// API Overload 2: DataFrame provided
//-----------------------------------------------------------------
DataFrame<double> PredictInterval( DataFrame< double > data,
                                   std::string         pathOut,
                                   std::string         predictFile,
                                   std::string         lib,
                                   std::string         pred,
                                   int                 E,
                                   int                 tau,
                                   std::string         colNames,
                                   std::string         targetName,
                                   bool                embedded,
                                   bool                verbose,
                                   unsigned            nThreads ) {
    
    // Container for results
    DataFrame<double> Tp_rho( 10, 2, "Tp rho" );

    // build work queue
    EDM_Eval::WorkQueue wq( 10 );

    // Insert Tp values into work queue
    for ( auto i = 0; i < 10; i++ ) {
        wq[ i ] = i + 1;
    }

    unsigned maxThreads = std::thread::hardware_concurrency();
    if ( maxThreads < nThreads ) { nThreads = maxThreads; }
    if ( nThreads > 10 )         { nThreads = 10;         }
    
    // thread container
    std::vector< std::thread > threads;
    for ( unsigned i = 0; i < nThreads; ++i ) {
        threads.push_back( std::thread( SimplexThread,
                                        std::ref( wq ),
                                        std::ref( data ),
                                        std::ref( Tp_rho ),
                                        lib,
                                        pred,
                                        E,
                                        0,       // Tp must be 0
                                        tau,
                                        colNames,
                                        targetName,
                                        embedded,
                                        verbose ) );
    }
    
    // join threads
    for ( auto &thrd : threads ) {
        thrd.join();
    }
    
    // Reset counter in case other threads are spawned in the app
    std::atomic_store( &EDM_Eval::tp_count_i, std::size_t(0) );
    
    if ( predictFile.size() ) {
        Tp_rho.WriteData( pathOut, predictFile );
    }

    return Tp_rho;
}

//----------------------------------------------------------------
// Worker thread for EmbedDimension() and PredictInterval()
//----------------------------------------------------------------
void SimplexThread( EDM_Eval::WorkQueue &wq,
                    DataFrame< double > &data,
                    DataFrame< double > &DF_rho,
                    std::string          lib,
                    std::string          pred,
                    int                  E,       // one of E and Tp must be
                    int                  Tp,      // zero
                    int                  tau,
                    std::string          colNames,
                    std::string          targetName,
                    bool                 embedded,
                    bool                 verbose )
{
    if ( E != 0 and Tp != 0 ) {
        throw std::runtime_error("SimplexThread() One of E or Tp must be 0.\n");
    }
    
    std::size_t i;

    // Decide if E or Tp are in the WorkQueue
    bool wq_E  = false;
    bool wq_Tp = false;
    if ( E  < 1 ) {
        wq_E = true;
        i  = std::atomic_fetch_add( &EDM_Eval::embed_count_i, std::size_t(1) );
    }
    else if (Tp < 1) {
        wq_Tp = true;
        i  = std::atomic_fetch_add( &EDM_Eval::tp_count_i, std::size_t(1) );
    }
    
    while( i < wq.size() ) {
        
        // WorkQueue stores E or Tp
        if      ( wq_E  ) { E  = wq[ i ]; }
        else if ( wq_Tp ) { Tp = wq[ i ]; }
                  
        DataFrame<double> S = Simplex( data,
                                       "",          // pathOut,
                                       "",          // predictFile,
                                       lib,
                                       pred,
                                       E,
                                       Tp,
                                       0,           // knn = 0
                                       tau,
                                       colNames,
                                       targetName,
                                       embedded,
                                       verbose );
        
        VectorError ve = ComputeError( S.VectorColumnName( "Observations" ),
                                       S.VectorColumnName( "Predictions"  ) );

        if ( wq_E ) {
            DF_rho.WriteRow( i, std::valarray<double>({ (double) E, ve.rho }));
        }
        else if ( wq_Tp ) {
            DF_rho.WriteRow( i, std::valarray<double>({ (double) Tp, ve.rho }));
        }
        
        if ( verbose ) {
            std::unique_lock<std::mutex> lck( EDM_Eval::mtx );
            std::cout << "SimplexThread() wq_E(" << wq_E << ") wq_Tp(" << wq_Tp 
                      << ")  E " << E << "  Tp " << Tp 
                      << "  rho " << ve.rho << "  RMSE " << ve.RMSE
                      << "  MAE " << ve.MAE << std::endl << std::endl;
            lck.unlock();
        }
    
        if ( wq_E ) {
            i = std::atomic_fetch_add(&EDM_Eval::embed_count_i, std::size_t(1));
        }
        else if ( wq_Tp ) {
            i = std::atomic_fetch_add( &EDM_Eval::tp_count_i, std::size_t(1) );
        }
    }
}

//----------------------------------------------------------------
// PredictNonlinear() : Smap rho vs. localisation parameter theta
// API Overload 1: Explicit data file path/name
//     Implemented as a wrapper to API Overload 2: 
//----------------------------------------------------------------
DataFrame<double> PredictNonlinear( std::string pathIn,
                                    std::string dataFile,
                                    std::string pathOut,
                                    std::string predictFile,
                                    std::string lib,
                                    std::string pred,
                                    int         E,
                                    int         Tp,
                                    int         tau,
                                    std::string colNames,
                                    std::string targetName,
                                    bool        embedded,
                                    bool        verbose,
                                    unsigned    nThreads ) {
    
    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );
    
    DataFrame< double > Theta_rho = PredictNonlinear( dataFrameIn,
                                                      pathOut,
                                                      predictFile,
                                                      lib,
                                                      pred,
                                                      E,
                                                      Tp,
                                                      tau,
                                                      colNames,
                                                      targetName,
                                                      embedded,
                                                      verbose );
    return Theta_rho;
}

//----------------------------------------------------------------
// PredictNonlinear() : Smap rho vs. localisation parameter theta
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> PredictNonlinear( DataFrame< double > data,
                                    std::string         pathOut,
                                    std::string         predictFile,
                                    std::string         lib,
                                    std::string         pred,
                                    int                 E,
                                    int                 Tp,
                                    int                 tau,
                                    std::string         colNames,
                                    std::string         targetName,
                                    bool                embedded,
                                    bool                verbose,
                                    unsigned            nThreads ) {

    std::valarray<double> ThetaValues( { 0.01, 0.1, 0.3, 0.5, 0.75, 1,
                                          1.5, 2, 3, 4, 5, 6, 7, 8, 9 } );

    // Container for results
    DataFrame<double> Theta_rho( ThetaValues.size(), 2, "Theta rho" );

    // build work queue
    EDM_Eval::WorkQueue wq( ThetaValues.size() );

    // Insert ThetaValues indexes into work queue
    for ( auto i = 0; i < ThetaValues.size(); i++ ) {
        wq[ i ] = i;
    }

    unsigned maxThreads = std::thread::hardware_concurrency();
    if ( maxThreads < nThreads ) { nThreads = maxThreads; }
    if ( nThreads > 15 )         { nThreads = 15;         }
    
    // thread container
    std::vector< std::thread > threads;
    for ( unsigned i = 0; i < nThreads; ++i ) {
        threads.push_back( std::thread( SMapThread,
                                        std::ref( wq ),
                                        std::ref( data ),
                                        std::ref( Theta_rho ),
                                        ThetaValues,
                                        lib,
                                        pred,
                                        E,
                                        Tp,
                                        tau,
                                        colNames,
                                        targetName,
                                        embedded,
                                        verbose ) );
    }
    
    // join threads
    for ( auto &thrd : threads ) {
        thrd.join();
    }

    // Reset counter in case other threads are spawned in the app
    std::atomic_store( &EDM_Eval::smap_count_i, std::size_t(0) );
    
    if ( predictFile.size() ) {
        Theta_rho.WriteData( pathOut, predictFile );
    }
    
    return Theta_rho;
}

//----------------------------------------------------------------
// Worker thread for PredictNonlinear()
//----------------------------------------------------------------
void SMapThread( EDM_Eval::WorkQueue   &wq,
                 DataFrame< double >   &data,
                 DataFrame< double >   &Theta_rho,
                 std::valarray<double>  ThetaValues,
                 std::string            lib,
                 std::string            pred,
                 int                    E,
                 int                    Tp,
                 int                    tau,
                 std::string            colNames,
                 std::string            targetName,
                 bool                   embedded,
                 bool                   verbose )
{
    
    std::size_t i =
        std::atomic_fetch_add( &EDM_Eval::smap_count_i, std::size_t(1) );

    while( i < wq.size() ) {
        
        double theta = ThetaValues[ wq[ i ] ];  
        
        SMapValues S = SMap( data,
                             "",
                             "",      // predictFile
                             lib,
                             pred,
                             E,
                             Tp,
                             0,       // knn
                             tau,
                             theta,
                             colNames,
                             targetName,
                             "",      // smapFile
                             "",      // jacobians
                             embedded,
                             verbose );
        
        DataFrame< double > predictions  = S.predictions;
        DataFrame< double > coefficients = S.coefficients;
        
        VectorError ve = ComputeError(
            predictions.VectorColumnName( "Observations" ),
            predictions.VectorColumnName( "Predictions"  ) );

        Theta_rho.WriteRow( i, std::valarray<double>({ theta, ve.rho }));
        
        if ( verbose ) {
            std::unique_lock<std::mutex> lck( EDM_Eval::mtx );
            std::cout << "Theta " << theta
                      << "  rho " << ve.rho << "  RMSE " << ve.RMSE
                      << "  MAE " << ve.MAE << std::endl << std::endl;
            lck.unlock();
        }
    
        i = std::atomic_fetch_add( &EDM_Eval::smap_count_i, std::size_t(1) );
    }
}

#include "Common.h"

// TODO: Multithread these evaluations

//----------------------------------------------------------------
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
                                  bool        verbose ) {

    
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
                                              verbose );

    return E_rho;
}

//----------------------------------------------------------------
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> EmbedDimension( DataFrame< double > data,
                                  std::string pathOut,
                                  std::string predictFile,
                                  std::string lib,
                                  std::string pred,
                                  int         Tp,
                                  int         tau,
                                  std::string colNames,
                                  std::string targetName,
                                  bool        embedded,
                                  bool        verbose ) {

    
    // Container for results
    DataFrame<double> E_rho( 10, 2, "E rho" );

    for ( int e = 1; e < 11; e++ ) {
    
        DataFrame<double> S = Simplex( data,
                                       pathOut,
                                       "",          // predictFile,
                                       lib,
                                       pred,
                                       e,
                                       Tp,
                                       0,           // knn = 0
                                       tau,
                                       colNames,
                                       targetName,
                                       embedded,
                                       verbose );
    
        VectorError ve = ComputeError( S.VectorColumnName( "Observations" ),
                                       S.VectorColumnName( "Predictions"  ) );

        E_rho.WriteRow( e-1, std::valarray< double >({ (double) e, ve.rho }));

        if ( verbose ) {
            std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                      << "  MAE " << ve.MAE << std::endl << std::endl;
        }
    } // for ( int e = 1; e < 11; e++ )

    if ( predictFile.size() ) {
        E_rho.WriteData( pathOut, predictFile );
    }
    
    return E_rho;
}

//----------------------------------------------------------------
// API Overload 1: Explicit data file path/name
//     Implemented as a wrapper to API Overload 2: 
//----------------------------------------------------------------
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
                                   bool        verbose ) {

    
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

//----------------------------------------------------------------
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> PredictInterval( DataFrame< double > data,
                                   std::string pathOut,
                                   std::string predictFile,
                                   std::string lib,
                                   std::string pred,
                                   int         E,
                                   int         tau,
                                   std::string colNames,
                                   std::string targetName,
                                   bool        embedded,
                                   bool        verbose ) {

    
    // Container for results
    DataFrame<double> Tp_rho( 10, 2, "Tp rho" );

    for ( int t = 1; t < 11; t++ ) {
    
        DataFrame<double> S = Simplex( data,
                                       pathOut,
                                       "",          // predictFile,
                                       lib,
                                       pred,
                                       E,
                                       t,
                                       0,           // knn = 0
                                       tau,
                                       colNames,
                                       targetName,
                                       embedded,
                                       verbose );
    
        VectorError ve = ComputeError( S.VectorColumnName( "Observations" ),
                                       S.VectorColumnName( "Predictions"  ) );

        Tp_rho.WriteRow( t-1, std::valarray< double >({ (double) t, ve.rho }));

        if ( verbose ) {
            std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                      << "  MAE " << ve.MAE << std::endl << std::endl;
        }
    } // for ( int t = 1; t < 11; t++ )

    if ( predictFile.size() ) {
        Tp_rho.WriteData( pathOut, predictFile );
    }
    
    return Tp_rho;
}


//----------------------------------------------------------------
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
                                    bool        verbose ) {

    
    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );
    
    DataFrame<double> Tp_rho = PredictNonlinear( dataFrameIn,
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

    return Tp_rho;
}

//----------------------------------------------------------------
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> PredictNonlinear( DataFrame< double > data,
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
                                    bool        verbose ) {

    
    std::valarray<double>  ThetaValues( { 0.01, 0.1, 0.3, 0.5, 0.75, 1,
                                           1.5, 2, 3, 4, 5, 6, 7, 8, 9 } );

    // Container for results
    DataFrame<double> Theta_rho( ThetaValues.size(), 2, "Theta rho" );

    int row = 0;
    for ( auto theta : ThetaValues ) {
    
        SMapValues S = SMap( data,
                             pathOut,
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

        Theta_rho.WriteRow( row, std::valarray<double>({ theta, ve.rho }));
        row++;

        if ( verbose ) {
            std::cout << "rho " << ve.rho << "  RMSE " << ve.RMSE
                      << "  MAE " << ve.MAE << std::endl << std::endl;
        }
    } // for ( theta )

    if ( predictFile.size() ) {
        Theta_rho.WriteData( pathOut, predictFile );
    }
    
    return Theta_rho;
}

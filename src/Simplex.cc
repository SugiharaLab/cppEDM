
#include "Common.h"
#include "Parameter.h"
#include "DataIO.h"
#include "Neighbors.h"
#include "Embed.h"
#include "DataEmbedNN.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
DataFrame<double> Simplex( std::string pathIn,
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
                           bool        embedded,
                           bool        verbose ) {

    Parameters param = Parameters( Method::Simplex, pathIn, dataFile,
                                   pathOut, predictFile,
                                   lib, pred, E, Tp, knn, tau, 0,
                                   columns, target, embedded, verbose );

#ifdef REMOVE
    DataEmbedNN dataEmbedNN = LoadDataEmbedNN( param, columns );
    DataIO                dio        = dataEmbedNN.dio;
    DataFrame<double>     dataBlock  = dataEmbedNN.dataFrame;
    std::valarray<double> target_vec = dataEmbedNN.targetVec;
    Neighbors             neighbors  = dataEmbedNN.neighbors;
#endif

    //----------------------------------------------------------
    // Load data to DataIO
    //----------------------------------------------------------
    DataIO dio = DataIO( param.pathIn, param.dataFile );

    //----------------------------------------------------------
    // Extract or embedd data block
    //----------------------------------------------------------
    DataFrame<double> dataBlock; // Multivariate or embedded DataFrame

    if ( param.embedded ) {
        // Data is multivariable block, no embedding needed
        if ( param.columnNames.size() ) {
         dataBlock = dio.DFrame().DataFrameFromColumnNames(param.columnNames);
        }
        else if ( param.columnIndex.size() ) {
         dataBlock = dio.DFrame().DataFrameFromColumnIndex(param.columnIndex);
        }
        else {
            throw std::runtime_error( "Simplex(): colNames and colIndex "
                                      " are empty" );
        }
    }
    else {
        // embedded = false: create the embedding block
        dataBlock = Embed( param.pathIn, param.dataFile,
                           param.E, param.tau,
                           columns, param.verbose );
    }
    
    //----------------------------------------------------------
    // Get target (library) vector
    //----------------------------------------------------------
    std::valarray<double> target_vec;
    if ( param.targetIndex ) {
        target_vec = dio.DFrame().Column( param.targetIndex );
    }
    else if ( param.targetName.size() ) {
        target_vec = dio.DFrame().VectorColumnName( param.targetName );
    }
    else {
        // Default to first column, column i=0 is time
        target_vec = dio.DFrame().Column( 1 );
    }

    //----------------------------------------------------------
    // Nearest neighbors
    //----------------------------------------------------------
    // If knn not specified, knn set to E+1
    if ( param.knn < 1 ) {
        param.knn = param.E + 1;
    }
    if ( param.knn < param.E + 1 ) {
        std::stringstream errMsg;
        errMsg << "Simplex(): knn of " << param.knn << " is less than E+1 of "
               << param.E + 1 << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    
    const Parameters        &param_ = param;
    const DataFrame<double> &D_     = dataBlock;
    Neighbors neighbors = FindNeighbors( D_, param_ );

    //----------------------------------------------------------
    // Simplex projection
    //----------------------------------------------------------
    size_t library_N_row = param.library.size();
    size_t N_row         = neighbors.neighbors.NRows();

    if ( N_row != neighbors.distances.NRows() ) {
        std::stringstream errMsg;
        errMsg << "Simplex(): Number of neighbor rows " << N_row
               << " doesn't match the number of distances rows "
               << neighbors.distances.NRows() << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    double minWeight = 1.E-6;
    std::valarray<double> predictions( 0., N_row );

    // Process each prediction row in neighbors
    for ( size_t row = 0; row < N_row; row++ ) {

        std::valarray<double> distanceRow = neighbors.distances.Row( row );
        
        // Establish exponential weight reference, the 'distance scale'
        double minDistance = distanceRow.min();

        // Compute weight (vector) for each k_NN
        std::valarray<double> weightedDistances( minWeight, param.knn );
        
        if ( minDistance == 0 ) {
            // Handle cases of distanceRow = 0 : can't divide by minDistance
            for ( size_t i = 0; i < param.knn; i++ ) {
                if ( distanceRow[i] > 0 ) {
                    weightedDistances[i] = exp( -distanceRow[i] / minDistance );
                }
                else {
                    // Setting weight = 1 implies that the corresponding
                    // library target vector is the same as the observation
                    // so it will be given full-weight in the prediction.
                    weightedDistances[i] = 1;
                }
            }
        }
        else {
            // exp() is a valarray<> overload (vectorized?)
            weightedDistances = exp( -distanceRow / minDistance );
        }

        // weight vector
        std::valarray<double> weights( param.knn );
        for  ( size_t i = 0; i < param.knn; i++ ) {
            weights[i] = std::max( weightedDistances[i], minWeight );
        }

        // target library vector, one element for each knn
        std::valarray<double> libTarget( param.knn );

        for ( size_t k = 0; k < param.knn; k++ ) {
            double libRow = neighbors.neighbors( row, k ) + param.Tp;

            if ( libRow > library_N_row ) {
                // The k_NN index + Tp is outside the library domain
                // Can only happen if noNeighborLimit = true is used.
                if ( param.verbose ) {
                    std::stringstream msg;
                    msg << "Simplex() in row " << row << " libRow " << libRow
                        << " exceeds library domain.\n";
                    std::cout << msg.str();
                }
                
                // Use the neighbor at the 'base' of the trajectory
                libTarget[ k ] = target_vec[ libRow - param.Tp ];
            }
            else {
                libTarget[ k ] = target_vec[ libRow ];
            }
        }

        // Prediction is average of weighted library projections
        predictions[ row ] = ( weights * libTarget ).sum() / weights.sum();
        
    } // for ( row = 0; row < N_row; row++ )

    //----------------------------------------------------
    // Ouput
    //----------------------------------------------------
    std::slice pred_i = std::slice( param.prediction[0], N_row, 1 );
    
    // Time vector with additional Tp points
    //----------------------------------------------------
    std::valarray<double> time( N_row + param.Tp );
    
    // Insert times from prediction. Time is the 1st column
    time = dio.DFrame().Column( 0 )[ pred_i ];
    // Insert Tp times at end
    for ( size_t i = N_row; i < N_row + param.Tp; i++ ) {
        time[ i ] = time[ i - 1 ] + param.Tp;
    }

    // Observations: add Tp nan at end
    //----------------------------------------------------
    std::valarray<double> observations( N_row + param.Tp );
    observations[ std::slice( 0, N_row, 1 ) ] = target_vec[ pred_i ];
    for ( size_t i = N_row; i < N_row + param.Tp; i++ ) {
        observations[ i ] = NAN;
    }

    // Predictions: insert Tp nan at start
    //----------------------------------------------------
    std::valarray<double> predictionsOut( N_row + param.Tp );
    for ( size_t i = 0; i < param.Tp; i++ ) {
        predictionsOut[ i ] = NAN;
    }
    predictionsOut[ std::slice(param.Tp, N_row, 1) ] = predictions;

    // Create output DataFrame
    DataFrame<double> dataFrame( N_row + param.Tp, 3 );
    dataFrame.ColumnNames() = { "Time", "Observations", "Predictions" };
    dataFrame.WriteColumn( 0, time );
    dataFrame.WriteColumn( 1, observations );
    dataFrame.WriteColumn( 2, predictionsOut );

    if ( param.predictOutputFile.size() ) {
        // Write to disk, first embed in a DataIO object
        DataIO dout( dataFrame );
        dout.WriteData( param.pathOut, param.predictOutputFile );
    }
    
#ifdef DEBUG_ALL
    std::cout << "Simplex -----------------------------------\n";
    std::cout << "time \tobserve \tpredict\n";
    for ( size_t row = 0; row < N_row + param.Tp; row++ ) {
        std::cout << time[row] << " \t"
                  << observations[ row ] << " \t"
                  << predictionsOut[ row ] << std::endl;
    }
    std::cout << "-------------------------------------------\n";
#endif
    
    return dataFrame;
}


#include "Common.h"
#include "Parameter.h"
#include "Neighbors.h"
#include "Embed.h"
#include "AuxFunc.h"

// Forward declaration
DataFrame<double> SimplexProjection( Parameters param, DataEmbedNN embedNN );

//----------------------------------------------------------------
// API Overload 1: Explicit data file path/name
//   Implemented as a wrapper to API Overload 2:
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
    
    // Create DataFrame (constructor loads data)
    DataFrame< double > dataFrameIn( pathIn, dataFile );

    // Pass data frame to Simplex 
    DataFrame< double > S = Simplex( dataFrameIn, pathOut,
                                     predictFile, lib, pred,
                                     E, Tp, knn, tau,
                                     columns, target,
                                     embedded, verbose);
    return S;
}

//----------------------------------------------------------------
// API Overload 2: DataFrame provided
//----------------------------------------------------------------
DataFrame<double> Simplex( DataFrame< double > data,
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

    Parameters param = Parameters( Method::Simplex, "", "",
                                   pathOut, predictFile,
                                   lib, pred, E, Tp, knn, tau, 0,
                                   columns, target, embedded, verbose );

    //----------------------------------------------------------
    // Embed, compute Neighbors
    //----------------------------------------------------------
    DataEmbedNN           embedNN    = EmbedNN( data, param, columns );
    DataFrame<double>     dataIn     = embedNN.dataIn;
    DataFrame<double>     dataBlock  = embedNN.dataFrame;
    std::valarray<double> target_vec = embedNN.targetVec;
    Neighbors             neighbors  = embedNN.neighbors;

    DataFrame<double> S = SimplexProjection( param, embedNN );

    return S;
}

//----------------------------------------------------------------
// Simplex Projection
//----------------------------------------------------------------
DataFrame<double> SimplexProjection( Parameters param, DataEmbedNN embedNN ) {

    // Unpack the data, embedding (dataBlock), target & neighbors
    DataFrame<double>     dataIn     = embedNN.dataIn;
    DataFrame<double>     dataBlock  = embedNN.dataFrame;
    std::valarray<double> target_vec = embedNN.targetVec;
    Neighbors             neighbors  = embedNN.neighbors;

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
    DataFrame<double> dataFrame = FormatOutput( param, N_row, predictions, 
                                                dataIn, target_vec );

    if ( param.predictOutputFile.size() ) {
        // Write to disk
        dataFrame.WriteData( param.pathOut, param.predictOutputFile );
    }
    
#ifdef DEBUG_ALL
    std::cout.precision( 4 );
    std::cout.fill( ' ' );
    std::cout.setf( std::ios::fixed, std::ios::floatfield );
    std::cout << "Simplex -----------------------------------\n";
    std::cout << std::setw(13) << "Time" << std::setw(13) << "Observe"
              << std::setw(13) << "Predict\n";
    for ( size_t row = 0; row < N_row + param.Tp; row++ ) {
        std::cout std::setw(13) << time[row] << " \t"
                  std::setw(13) << observations[ row ] << " \t"
                  std::setw(13) << predictionsOut[ row ] << std::endl;
    }
    std::cout << "-------------------------------------------\n";
#endif
    
    return dataFrame;
}

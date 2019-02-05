
#include "Common.h"
#include "Parameter.h"
#include "Neighbors.h"
#include "Embed.h"
#include "AuxFunc.h"

//----------------------------------------------------------------
// 
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
    // Load data, Embed, compute Neighbors
    //----------------------------------------------------------
    DataEmbedNN dataEmbedNN = LoadDataEmbedNN( data, param, columns );
    DataFrame<double>     originalData  = dataEmbedNN.originalData;
    DataFrame<double>     dataBlock     = dataEmbedNN.dataFrame;
    std::valarray<double> target_vec    = dataEmbedNN.targetVec;
    Neighbors             neighbors     = dataEmbedNN.neighbors;

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
    DataFrame<double> dataFrame = FormatOutput( param, N_row, predictions, 
                                                originalData, target_vec );

    if ( param.predictOutputFile.size() ) {
        // Write to disk, first embed in a DataIO object
        dataFrame.WriteData( param.pathOut, param.predictOutputFile );
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
    //create DataFrame and delegate
    DataFrame< double > toSimplex (pathIn, dataFile);
    DataFrame< double > simplexOutput = Simplex (toSimplex, pathOut,
                                        predictFile, lib, pred, E, Tp, knn, tau,
                                        columns, target, embedded, verbose);
    return simplexOutput;
}


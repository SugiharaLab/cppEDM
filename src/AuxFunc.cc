
#include "AuxFunc.h"

//----------------------------------------------------------
// Common code to Simplex and Smap that loads data,
// embeds, computes neighbors.
//----------------------------------------------------------
DataEmbedNN LoadDataEmbedNN( DataFrame<double> originalData,
                             Parameters  param,
                             std::string columns ) {

    //----------------------------------------------------------
    // Extract or embedd data block
    //----------------------------------------------------------
    DataFrame<double> dataBlock; // Multivariate or embedded DataFrame

    if ( param.embedded ) {
        // Data is multivariable block, no embedding needed
        if ( param.columnNames.size() ) {
         dataBlock = originalData.DataFrameFromColumnNames(param.columnNames);
        }
        else if ( param.columnIndex.size() ) {
         dataBlock = originalData.DataFrameFromColumnIndex(param.columnIndex);
        }
        else {
            throw std::runtime_error( "LoadDataEmbedNN(): colNames and colIndex "
                                      " are empty" );
        }
    }
    else {
        // embedded = false: create the embedding block
        dataBlock = Embed( originalData,
                           param.E,      param.tau,
                           columns,      param.verbose );
    }
    
    //----------------------------------------------------------
    // Get target (library) vector
    //----------------------------------------------------------
    std::valarray<double> target_vec;
    if ( param.targetIndex ) {
        target_vec = originalData.Column( param.targetIndex );
    }
    else if ( param.targetName.size() ) {
        target_vec = originalData.VectorColumnName( param.targetName );
    }
    else {
        // Default to first column, column i=0 is time
        target_vec = originalData.Column( 1 );
    }

    //----------------------------------------------------------
    // Nearest neighbors
    //----------------------------------------------------------
    Neighbors neighbors = FindNeighbors( dataBlock, param );

    // Create struct to return the objects
    DataEmbedNN dataEmbedNN = DataEmbedNN(originalData, dataBlock, 
                                          target_vec, neighbors);

    return dataEmbedNN;
}

//----------------------------------------------------------
// Common code to Simplex and Smap for output generation
//----------------------------------------------------------
DataFrame<double> FormatOutput( Parameters            param,
                                size_t                N_row,
                                std::valarray<double> predictions,
                                DataFrame<double>     dataFrameIn,
                                std::valarray<double> target_vec )
{

    std::slice pred_i = std::slice( param.prediction[0], N_row, 1 );
    
    // Time vector with additional Tp points
    //----------------------------------------------------
    std::valarray<double> time( N_row + param.Tp );
    
    // Insert times from prediction. Time is the 1st column
    time = dataFrameIn.Column( 0 )[ pred_i ];
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
    
    return dataFrame;
}

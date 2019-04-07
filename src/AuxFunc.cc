
#include "AuxFunc.h"

//----------------------------------------------------------
// Common code for Simplex and Smap that embeds, extracts
// the target vector and computes neighbors.
// Note that the time column is not returned in the embedding
// dataBlock.
//----------------------------------------------------------
DataEmbedNN EmbedNN( DataFrame<double> dataIn,
                     Parameters        param  )
{
    //----------------------------------------------------------
    // Extract or embedd data block
    //----------------------------------------------------------
    DataFrame<double> dataBlock; // Multivariate or embedded DataFrame

    if ( param.embedded ) {
        // Data is multivariable block, no embedding needed
        // Select the specified columns 
        if ( param.columnNames.size() ) {
            dataBlock = dataIn.DataFrameFromColumnNames(param.columnNames);
        }
        else if ( param.columnIndex.size() ) {
            dataBlock = dataIn.DataFrameFromColumnIndex(param.columnIndex);
        }
        else {
            throw std::runtime_error( "EmbedNN(): colNames and "
                                      " colIndex are empty.\n" );
        }
    }
    else {
        // embedded = false: Create the embedding block
        // dataBlock will have tau * (E-1) fewer rows than dataIn
        dataBlock = Embed( dataIn, param.E, param.tau,
                           param.columns_str, param.verbose );
    }
    
    //----------------------------------------------------------
    // Get target (library) vector
    //----------------------------------------------------------
    std::valarray<double> target_vec;
    if ( param.targetIndex ) {
        target_vec = dataIn.Column( param.targetIndex );
    }
    else if ( param.targetName.size() ) {
        target_vec = dataIn.VectorColumnName( param.targetName );
    }
    else {
        // Default to first column, column i=0 is time
        target_vec = dataIn.Column( 1 );
    }
    
    //----------------------------------------------------------
    // If data was embedded, remove dataIn, target rows as needed
    //----------------------------------------------------------
    if ( not param.embedded ) {
        // If we support negtive tau, this will change
        // For now, assume only positive tau is allowed
        size_t shift = std::max(0, param.tau * (param.E - 1) );
        
        std::valarray<double> target_vec_embed( dataIn.NRows() - shift );
        target_vec_embed = target_vec[ std::slice( shift,
                                                   target_vec.size() - shift,
                                                   1 ) ];
        target_vec = target_vec_embed;

        DataFrame<double> dataInEmbed( dataIn.NRows() - shift,
                                       dataIn.NColumns(),
                                       dataIn.ColumnNames() );
        
        for ( size_t row = 0; row < dataInEmbed.NRows(); row++ ) {
            dataInEmbed.WriteRow( row, dataIn.Row( row + shift ) );
        }
        dataIn = dataInEmbed;
    }
    
    //----------------------------------------------------------
    // Nearest neighbors
    //----------------------------------------------------------
    Neighbors neighbors = FindNeighbors( dataBlock, param );

    // Create struct to return the objects
    DataEmbedNN dataEmbedNN = DataEmbedNN( dataIn, dataBlock, 
                                           target_vec, neighbors );
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

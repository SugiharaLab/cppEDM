
#include "DataEmbedNN.h"

//----------------------------------------------------------
// Common code to Simplex and Smap that loads data,
// embeds, computes neighbors.
// Data objects seem right, but when used in Simplex the
// predictions are nan...
//----------------------------------------------------------
DataEmbedNN LoadDataEmbedNN( Parameters  param,
                             std::string columns) {

    //----------------------------------------------------------
    // Load data to DataIO
    //----------------------------------------------------------
    DataIO dio = DataIO( param.path, param.dataFile );

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
            throw std::runtime_error( "LoadDataEmbedNN(): colNames and colIndex "
                                      " are empty" );
        }
    }
    else {
        // embedded = false: create the embedding block
        dataBlock = Embed( param.path, param.dataFile,
                           param.E,    param.tau,
                           columns,    param.verbose );
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

    // Create struct to return the objects
    DataEmbedNN dataEmbedNN = DataEmbedNN();

    dataEmbedNN.dio       = dio;
    dataEmbedNN.dataFrame = dataBlock;
    dataEmbedNN.targetVec = target_vec;
    dataEmbedNN.neighbors = neighbors;

    return dataEmbedNN;
}

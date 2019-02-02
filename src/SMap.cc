
#include "Common.h"
#include "Parameter.h"
#include "DataIO.h"
#include "Embed.h"
#include "Neighbors.h"
#include "DataEmbedNN.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
DataFrame<double> SMap( std::string pathIn,
                        std::string dataFile,
                        std::string pathOut,
                        std::string predictFile,
                        std::string lib,
                        std::string pred,
                        int         E,
                        int         Tp,
                        int         knn,
                        int         tau,
                        double      theta,
                        std::string columns,
                        std::string target,
                        bool        embedded,
                        bool        verbose,
                        std::string smapFile,
                        std::string jacobians,
                        double      SVDsignificance ) {

    Parameters param = Parameters( Method::SMap, pathIn, dataFile,
                                   pathOut, predictFile,
                                   lib, pred, E, Tp, knn, tau, theta,
                                   columns, target, embedded, verbose,
                                   smapFile, "", jacobians, SVDsignificance );

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
            throw std::runtime_error( "SMap(): colNames and colIndex "
                                      " are empty" );
        }
    }
    else {
        // embedded = false: create the embedding block
        dataBlock = Embed( param.pathIn, param.dataFile,
                           param.E,      param.tau,
                           columns,      param.verbose );
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
    // knn was checked in Parameters::Validate()
    const Parameters        &param_ = param;
    const DataFrame<double> &D_     = dataBlock;
    Neighbors neighbors = FindNeighbors( D_, param_ );

    //----------------------------------------------------------
    // SMap projection
    //----------------------------------------------------------
    
    
}

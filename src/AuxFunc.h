#ifndef AUXFUNC
#define AUXFUNC

#include "Neighbors.h"
#include "Embed.h"

//----------------------------------------------------------------
// Data Input, embedding and NN structure to accomodate
// common initial processing in Simplex and Smap
//----------------------------------------------------------------
struct DataEmbedNN {

    DataFrame<double>     dataFrame;
    std::valarray<double> targetVec;
    Neighbors             neighbors;
    
    // Constructor
    DataEmbedNN( DataFrame<double>     dataFrame,
                 std::valarray<double> targetVec,
                 Neighbors             neighbors ) :
        dataFrame( dataFrame ), targetVec( targetVec ),
        neighbors( neighbors ) {}
};

DataEmbedNN LoadDataEmbedNN( DataFrame< double > data,
                             Parameters  param, std::string columns );
    
DataFrame<double> FormatOutput( Parameters            param,
                                size_t                N_row,
                                std::valarray<double> predictions,
                                DataFrame<double>     dataFrameIn,
                                std::valarray<double> target_vec );
#endif

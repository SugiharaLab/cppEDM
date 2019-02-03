#ifndef AUXFUNC
#define AUXFUNC

#include "DataIO.h"
#include "Neighbors.h"
#include "Embed.h"

//----------------------------------------------------------------
// Data Input, embedding and NN structure to accomodate
// common initial processing in Simplex and Smap
//----------------------------------------------------------------
struct DataEmbedNN {
    DataIO                dio;
    DataFrame<double>     dataFrame;
    std::valarray<double> targetVec;
    Neighbors             neighbors;
    
    // Constructor
    DataEmbedNN( DataIO                dio,
                 DataFrame<double>     dataFrame,
                 std::valarray<double> targetVec,
                 Neighbors             neighbors ) :
        dio( dio ), dataFrame( dataFrame ), targetVec( targetVec ),
        neighbors( neighbors ) {}
};

DataEmbedNN LoadDataEmbedNN( Parameters  param,
                             std::string columns );
    
DataFrame<double> FormatOutput( Parameters            param,
                                size_t                N_row,
                                std::valarray<double> predictions,
                                DataFrame<double>     dataFrameIn,
                                std::valarray<double> target_vec );
#endif

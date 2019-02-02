#ifndef DATAEMBEDNN
#define DATAEMBEDNN

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
};

DataEmbedNN LoadDataEmbedNN( Parameters  param,
                             std::string columns );
    
#endif

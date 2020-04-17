// Contains class definitions for EDM object. See EDM.h header for 
// description of EDM class and how to use the methods.

#include "EDM.h"

//----------------------------------------------------------------
// EDM()    : Constructor
//
// data     : Input dataframe containing the time series to model.
// embedded : Flag on whether the input data is already embedded.
// E        : The embedding dimension to use when performing embedding.
// tau      : The steps between each index in the embedding. Negative
//            tau is probably what you intend to use; positive tau yields
//            an embedding (E_t,E_t+tau...) (future forward embedding).
//----------------------------------------------------------------
EDM::EDM ( DataFrame<double> & data, bool embedded, int E, int tau ) : 
    data( data ) {

    // check parameters
     
}

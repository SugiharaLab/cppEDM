// Contains class declaration for EDM object. The EDM object is a general
// data-processing class which holds the data object and its embedding,
// and will have its specific projection methods defined by Simplex and SMap.

#ifndef EDM_H
#define EDM_H

#include "Common.h"

//----------------------------------------------------------------
// EDM Class
// EDM maintains the central data object used for prediction and
// defines data iterating patterns common to Simplex and SMap.
// It should be treated as an abstract class as it performs no
// prediction, but for sake of compiler simplicity we are choosing
// to leave it as a virtual class for now
//----------------------------------------------------------------
class EDM {

    // The input time series (potentially not embedded)
    DataFrame<double> & data;

    // The target column this class should model for
    std::string targetName;

    public:

        //----------------------------------------------------------------
        // EDM() : Constructor
        //----------------------------------------------------------------
        EDM ( DataFrame<double> & data, bool embedded, int E, int tau );

};

#endif

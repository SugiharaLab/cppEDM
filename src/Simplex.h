
#ifndef EDM_SIMPLEX_H
#define EDM_SIMPLEX_H

#include "EDM.h"

//----------------------------------------------------------------
// Simplex class inherits from EDM class and defines
// Simplex-specific projection methods
//----------------------------------------------------------------
class SimplexClass : public EDM {
public:
    // CCMClass includes two instances of SimplexClass. One for
    // forward mapping, one for reverse. These objects hold the
    // original input data subsetted for each library size.
    DataFrame    < double > dataCCM;   // Original, full data 
    std::valarray< double > targetCCM; // Original, full target
    
    // Constructor
    SimplexClass ( DataFrame<double> & data,
                   Parameters        & parameters );

    // Method declarations
    void Project();
    void Simplex();
    void CopyData(); // CCMClass 
    void WriteOutput();
};
#endif

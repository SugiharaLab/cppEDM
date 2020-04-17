// Contains the Simplex class declarations
// The Simplex class provides Simplex-algorithm-specific definitions for the 
// EDM class data processing methods

#include "Common.h"

//----------------------------------------------------------------
// Simplex class
// The Simplex class inherits from the EDM class and defines the 
// Simplex-specific projection methods for its parent EDM class
//----------------------------------------------------------------
class Simplex {

    public:

        //----------------------------------------------------------------
        // Simplex() : Constructor
        //----------------------------------------------------------------
        Simplex ( DataFrame<double> & data, bool embedded, int E, int tau );

};

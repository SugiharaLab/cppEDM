// Contains the Simplex class declarations
// The Simplex class provides Simplex-algorithm-specific definitions for the 
// EDM class data processing methods

#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "Common.h"
#include "EDM.h"

//----------------------------------------------------------------
// Simplex class
// The Simplex class inherits from the EDM class and defines the 
// Simplex-specific projection methods for its parent EDM class
//----------------------------------------------------------------
class SimplexMachine : public EDM {

    public:

        //----------------------------------------------------------------
        // Simplex() : Constructor with DataFrame input
        //----------------------------------------------------------------
        SimplexMachine ( DataFrame<double> &data, 
                std::string pathOut, std::string predictFile,
                std::string lib, std::string pred, 
                int E, int Tp, int knn, int tau, int exclusionRadius, 
                std::string columns, std::string target,
                bool embedded, bool const_predict, bool verbose  );

};

#endif

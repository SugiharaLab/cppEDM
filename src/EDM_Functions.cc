// These functions are function-wrappers for the EDM algorithm classes 
// such that the user does not have to deal with the OOD.

#include "Common.h"
#include "Simplex.h"

namespace cppEDM { 

    //----------------------------------------------------------------
    // Simplex prediction algorithm with data input as DataFrame
    // See EDM class for parameter definitions.
    //----------------------------------------------------------------
    DataFrame<double> Simplex( DataFrame<double> & data,
            std::string pathOut, std::string predictFile,
            std::string lib, std::string pred,
            int E, int Tp, int knn, int tau, int exclusionRadius,
            std::string columns, std::string target,
            bool embedded, bool const_predict, bool verbose ) {

        // Create Simplex object and get its projection

        SimplexMachine simplexManager( data, pathOut, predictFile, 
           lib, pred, E, Tp, knn, tau, exclusionRadius, columns, target, 
           embedded, const_predict, verbose );

        return DataFrame<double>();

    }

    //----------------------------------------------------------------
    // Simplex prediction algorithm with data input as filepath
    // See EDM class for parameter definitions.
    //----------------------------------------------------------------
    DataFrame<double> Simplex( std::string pathIn, std::string dataFile, 
            std::string pathOut, std::string predictFile,
            std::string lib, std::string pred,
            int E, int Tp, int knn, int tau, int exclusionRadius,
            std::string columns, std::string target,
            bool embedded, bool const_predict, bool verbose ) {

        // Just load in dataframe and delegate to Simplex with DataFrame input 

        DataFrame<double> data (pathIn, dataFile);

        return cppEDM::Simplex ( data, pathOut, predictFile, lib, pred, E, Tp, knn, tau, 
           exclusionRadius, columns, target, embedded, const_predict, verbose );
    }

}

// Definitions for the Simplex class

#include "Simplex.h"

//----------------------------------------------------------------
// Simplex() : Constructor
// See EDM class for descriptions of parameters not described
//----------------------------------------------------------------
SimplexMachine::SimplexMachine ( DataFrame<double> & data, 
                  std::string pathOut, std::string predictFile,
                  std::string lib, std::string pred, 
                  int E, int Tp, int knn, int tau, int exclusionRadius, 
                  std::string columns, std::string target,
                  bool embedded, bool const_predict, bool verbose  ):
                  EDM{data, E, tau, columns, target, embedded, verbose } {

        Project(lib,pred,Tp,knn,exclusionRadius,verbose);


}

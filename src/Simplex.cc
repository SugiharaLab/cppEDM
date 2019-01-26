
#include "Common.h"
#include "Parameter.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
void Simplex( std::string path         = "./data/",
              std::string dataFile     = "block_3sp.csv",
              std::string predictFile  = "",
              std::string lib          = "1 10",
              std::string pred         = "11 20",
              int         E            = 3,
              int         Tp           = 1,
              int         knn          = 0,
              int         tau          = 1,
              std::string colNames     = "x_t y_t z_t",
              std::string targetName   = "x_t",
              std::string colIndex     = "",
              int         targetIndex  = 0,
              bool        embedded     = false,
              bool        verbose      = true   ) {

    Parameters param = Parameters( Method::Simplex, path, dataFile, predictFile,
                                   lib, pred, E, Tp, knn, tau, 0,
                                   colNames, targetName, colIndex, targetIndex,
                                   embedded, verbose );

    // Load the data into a DataFrame

    // If requested (embedded = false) create the embedding block

    // Get the target (library) vector
    // Default to first data column

    // Get the observation (prediction) time vector
    
    // Get observation (prediction) vector
    // Default to first data column

    // Nearest neighbors


    // Simplex projection
    
}

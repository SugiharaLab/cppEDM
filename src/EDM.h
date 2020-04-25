// Contains class declaration for EDM object. The EDM object is a general
// data-processing class which holds the data object and its embedding,
// and will have its specific projection methods defined by Simplex and SMap.

#ifndef EDM_H
#define EDM_H

#include "Common.h"
#include "Embed.h"

//----------------------------------------------------------------
// EDM Class
// EDM maintains the central data object used for prediction and
// defines data iterating patterns common to Simplex and SMap.
// It should be treated as an abstract class as it performs no
// prediction, but for sake of compiler simplicity we are choosing
// to leave it as a virtual class for now
//----------------------------------------------------------------
class EDM {

    // The input time series and its embedding
    DataFrame<double> & data;
    DataFrame<double> embedding;

    // The dimension to be project onto
    std::string targetName;

    // Flag on whether to have no neighbor limit in neighbor search
    bool noNeighborLimit = false;

    // Return structure of FindNeighbors()
    struct Neighbors {
        DataFrame<size_t> neighbors;
        DataFrame<double> distances;
        Neighbors();
        ~Neighbors();
    };

    public:

    //----------------------------------------------------------------
    // EDM() : Constructor
    //----------------------------------------------------------------
    EDM ( DataFrame<double> & data, int E, int tau, 
            std::string columns, std::string targetName, 
            bool embedded, bool verbose  );

    //----------------------------------------------------------------
    // EDM() : ComputeNeighbors
    //----------------------------------------------------------------
    Neighbors ComputeNeighbors (  
            std::string lib, std::string pred, int Tp, int knn, 
            int exclusionRadius, bool verbose  );

    //----------------------------------------------------------------
    // EDM() : Project
    //----------------------------------------------------------------
    std::list< DataFrame<double> > Project ( std::string lib, std::string pred,
             int Tp, int knn, int exclusionRadius, bool verbose );

};

#endif

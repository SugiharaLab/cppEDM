#ifndef PARAMETER_H
#define PARAMETER_H

#include <algorithm>

#include "Common.h"

//------------------------------------------------------------
//
//------------------------------------------------------------
class Parameters {

public:  // JP Should be protected with accessors...
    Method      method;           // Simplex or SMap enum class
    std::vector<int> library;     // library row indices
    std::vector<int> prediction;  // prediction row indices
    int         E;                // dimension
    int         Tp;               // prediction interval
    int         knn;              // k nearest neighbors
    int         tau;              // block embedding delay
    
    float       theta;            // S Map localization
    float       SVDSignificance;  // SVD singular value cutoff
    std::vector<int> jacobians;        // list of column indices for Jacobians
    float       TikhonovAlpha;    // Initial alpha parameter
    float       ElasticNetAlpha;  // Initial alpha parameter

    std::vector<std::string> columnNames; // column names 
    std::vector<int>         columnIndex; // column indices

    std::string targetName;       // target column name
    int         targetIndex;      // target column index

    bool        embedded;         // true if data is already embedded/block
    
    int         MultiviewEnsemble;// Number of ensembles in multiview

    std::vector<int> librarySizes;// CCM library sizes to evaluate
    int         subSamples;       // CCM number of samples to draw
    bool        randomLib;        // CCM randomly select subsets if true
    int         seed;             // CCM random selection RNG seed
    
    bool        noNeighborLimit;  // Strictly forbid neighbors outside library
    bool        forwardTau;       // Embed/block with t+tau instead t-tau

    bool        verbose;

    std::string path;
    std::string dataFile;
    std::string predictOutputFile;
    std::string SmapOutputFile;
    std::string blockOutputFile;
    
public:
    Parameters();
    ~Parameters();

    void Load();   // Populate the parameters from arguments
    void Adjust(); // Parameter validation and index offsets
};

#endif

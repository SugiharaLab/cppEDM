#ifndef PARAMETER_H
#define PARAMETER_H

#include <algorithm>

#include "Common.h"

//------------------------------------------------------------
//
//------------------------------------------------------------
class EDM_Parameters {

protected:
    Method      method;           // Simplex or SMap enum class
    vector<int> library;          // library row indices
    vector<int> prediction;       // prediction row indices
    int         E;                // dimension
    int         Tp;               // prediction interval
    int         knn;              // k nearest neighbors
    int         tau;              // block embedding delay
    
    float       theta;            // S Map localization
    float       SVDSignificance;  // SVD singular value cutoff
    vector<int> jacobians;        // list of column indices for Jacobians
    float       TikhonovAlpha;    // Initial alpha parameter
    float       ElasticNetAlpha;  // Initial alpha parameter

    vector<string> columnNames;   // column names 
    vector<int>    columnIndex;   // column indices

    string      targetName;       // target column name
    int         targetIndex;      // target column index

    bool        embedded;         // true if data is already embedded/block
    
    int         MultiviewEnsemble;// Number of ensembles in multiview

    vector<int> librarySizes;     // CCM library sizes to evaluate
    int         subSamples;       // CCM number of samples to draw
    bool        randomLib;        // CCM randomly select subsets if true
    int         seed;             // CCM random selection RNG seed
    
    bool        noNeigborLimit;   // Strictly forbid neighbors outside library
    bool        forwardTau;       // Embed/block with t+tau instead t-tau

    string      path;
    string      dataFile;
    string      predictOutputFile;
    string      SmapOutputFile;
    string      blockOutputFile;
    
public:
    EDM_Parameters();
    ~EDM_Parameters();

    void Load();   // Populate the parameters from arguments
    void Adjust(); // Parameter validation and index offsets
};

#endif

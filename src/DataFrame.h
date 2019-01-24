#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <fstream>
#include <cctype>

#include "Common.h"
#include "Matrix.h"

// Prefix for DataFrame columns without header titles
#define COL_PREFIX "vec_"

#define MAX_PRINT_IDX 50 // DataFrame << max rows to print

//---------------------------------------------------------
// DataFrame class
// Provide a data storage container for EDM input data and
// embeddings (blocks) similar to the DataFrame of R/pandas
//---------------------------------------------------------
class DataFrame {

private:
    
    // Underlying datastructure is a single valarray<T>
    Matrix< double > container;
    
    NamedData csvInput; // map< string, vector<double> >

    std::vector< std::string > colNames; // colNames are ordered NamedData keys

    std::string path;     // User specified input path
    std::string fileName; // User specified input data file
    
public:

    DataFrame (const std::string path, const std::string fileName); 
    
    NamedData ReadData();
    
    Matrix< double > SetupContainer (const NamedData csvInput);

    Matrix< double > matrix() const { return container; }
    
    size_t NumColumns();
    size_t NumRows();
    
    std::vector< std::string > ColumnNames();
    
    friend std::ostream& operator<<(std::ostream& os, DataFrame& df);
    double & operator() (size_t rowIdx, size_t colIdx);
    double & operator() (size_t colIdx, std::string colName);
};

#endif

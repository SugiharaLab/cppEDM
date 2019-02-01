#ifndef DATAIO_H
#define DATAIO_H

#include <fstream>
#include <iterator>

#include "Common.h"

//---------------------------------------------------------
// DataIO class
// I/O wrapper for DataFrame
//---------------------------------------------------------
class DataIO {

private:
    
    // Underlying datastructure: a single valarray<T>
    DataFrame< double > dataFrame;
    
    NamedData csvInput; // pair< string, vector<double> >

    std::string path;     // User specified input path
    std::string fileName; // User specified input data file

    size_t maxRowPrint;
    
public:
    // Prototypes
    DataIO ( const std::string path,
             const std::string fileName,
             size_t maxPrint = 5 );
    
    NamedData ReadData();
    
    void WriteData(std::string outputFilePath, std::string outputFileName);
    
    DataFrame< double > SetupDataFrame ( const NamedData csvInput );

    // Accessors
    size_t NumColumns()  const { return dataFrame.NColumns(); }
    size_t NumRows()     const { return dataFrame.NRows();    }
    size_t MaxRowPrint() const { return maxRowPrint;          }
    
    DataFrame< double > &DFrame()        { return dataFrame; }
    DataFrame< double >  DFrame()  const { return dataFrame; }

    // Overloads
};

#endif

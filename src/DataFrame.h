#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <fstream>

#include "Common.h"

// Prefix for DataFrame columns without header titles
#define COL_PREFIX "V"

//---------------------------------------------------------
// DataFrame class
// Provide a data storage Matrix for EDM input data and
// embeddings (blocks) similar to the DataFrame of R/pandas
//---------------------------------------------------------
class DataFrame {

private:
    
    // Underlying datastructure: a single valarray<T>
    Matrix< double > dataMatrix;
    
    NamedData csvInput; // pair< string, vector<double> >

    std::vector< std::string > colNames;

    std::string path;     // User specified input path
    std::string fileName; // User specified input data file

    size_t maxRowPrint;
    
public:

    // Prototypes
    DataFrame ( const std::string path,
                const std::string fileName,
                size_t maxPrint = 5 );
    
    NamedData ReadData();
    bool WriteData(std::string outputFilePath, std::string outputFileName);
    
    Matrix< double > SetupDataMatrix ( const NamedData csvInput );

    Matrix< double > MatrixColumnNames( std::vector<std::string> colNames );
    std::valarray< double > VectorColumnName ( std::string colName );
    
    // Accessors
    size_t NumColumns()  const { return dataMatrix.NColumns(); }
    size_t NumRows()     const { return dataMatrix.NRows();    }
    size_t MaxRowPrint() const { return maxRowPrint;           }
    
    Matrix< double >          &DataMatrix()        { return dataMatrix; }
    Matrix< double >           DataMatrix()  const { return dataMatrix; }
    std::vector< std::string > ColumnNames() const { return colNames;   }

    // Overloads
    friend std::ostream& operator <<( std::ostream& os, DataFrame& df );
    double & operator() ( size_t rowIdx, size_t colIdx );
    double & operator() ( size_t colIdx, std::string colName );
};

#endif

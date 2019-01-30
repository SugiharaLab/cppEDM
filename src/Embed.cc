
#include "Embed.h"

//---------------------------------------------------------
// Function to embed Matrix columns in E dimensions.
// Data is read from path/dataFile
//
// side effects:        truncates the array by tau * (E-1) rows 
//                      to remove nan values (partial data rows)
// @param dataFile:     data file (csv)
// @param E:            embedding dimension
// @param tau:          time step delay
// @param columns:      column names or indices to embed
// @return:             Matrix with embedded data
//---------------------------------------------------------
Matrix< double > Embed ( std::string path,
                         std::string dataFile,
                         int         E,
                         int         tau,
                         std::string columns,
                         bool        verbose ) {
    
    // Parameter.Validate() will convert columns into a vector of names
    // or a vector of column indices
    Parameters param = Parameters( Method::Simplex, path, dataFile, "",
                                   "1 1", "1 1", E, 0, 0, tau, 0,
                                   columns, "", false, verbose );

    // Load dataMatrix with column from path/file
    // JP This is possibly redundant file I/O if called from another function
    DataFrame df = DataFrame( param.path, param.dataFile );
    
    // Extract the specified columns into a new dataMatrix for MakeBlock
    Matrix< double > dataMatrix;
    std::vector< std::string > colNames;

    if ( param.columnNames.size() ) {
        // columns names are strings, use as-is
        colNames   = param.columnNames;
        dataMatrix = df.MatrixColumnNames( param.columnNames );
    }
    else if ( param.columnIndex.size() ) {
        // columns are indices : Create column names for MakeBlock
        for ( size_t i = 0; i < param.columnIndex.size(); i++ ) {
            std::stringstream ss;
            ss << "V" << param.columnIndex[i];
            colNames.push_back( ss.str() );
        }
        dataMatrix = df.DataMatrix().MatrixColumns( param.columnIndex );
    }
    else {
        throw std::runtime_error( "Embed(file): columnNames and columnIndex "
                                  " are empty.\n" );
    }
    
    Matrix< double > embedding = MakeBlock( dataMatrix, E, tau,
                                            colNames, verbose );
    return embedding;
}

//---------------------------------------------------------
// Embed dataMatrix
// dataMatrix is passed in as a parameter
// Note: dataMatrix must have the columnNameToIndex map 
//---------------------------------------------------------
Matrix< double > Embed ( Matrix< double > dataMatrixIn,
                         int              E,
                         int              tau,
                         std::string      columns,
                         bool             verbose ) {

    // Parameter.Validate will convert columns into a vector of names
    // or a vector of column indices
    Parameters param = Parameters( Method::Simplex, "", "", "",
                                   "1 1", "1 1", E, 0, 0, tau, 0,
                                   columns, "", false, verbose );

    if ( not param.columnIndex.size() and
         dataMatrixIn.ColumnNameToIndex().empty() ) {
        throw std::runtime_error( "Embed(Matrix): columnNameIndex empty.\n" );
    }

    // Get column names for MakeBlock
    std::vector< std::string > colNames;
    if ( param.columnNames.size() ) {
        // column names are strings use as-is
        colNames = param.columnNames;
    }
    else if ( param.columnIndex.size() ) {
        // columns are indices : Create column names for MakeBlock
        for ( size_t i = 0; i < param.columnIndex.size(); i++ ) {
            std::stringstream ss;
            ss << "V" << param.columnIndex[i];
            colNames.push_back( ss.str() );
        }
    }
    else {
        throw std::runtime_error( "Embed(Matrix): columnNames and columnIndex "
                                  " are empty.\n" );
    }

    // Extract the specified columns (sub)Matrix from dataMatrixIn
    Matrix< double > dataMatrix;
    
    if ( param.columnNames.size() ) {
        // Get a vector of column indices 
        std::vector< size_t > col_i;
        for ( auto colName : param.columnNames ) {
            col_i.push_back( dataMatrixIn.ColumnNameToIndex()[ colName ] );
        }
        dataMatrix = dataMatrixIn.MatrixColumns( col_i );
    }
    else if ( param.columnIndex.size() ) {
        // alread have column indices
        dataMatrix = dataMatrixIn.MatrixColumns( param.columnIndex );
    }

    Matrix< double > embedding = MakeBlock( dataMatrix, E, tau,
                                            colNames, verbose );
    return embedding;
}

//---------------------------------------------------------
// MakeBlock from dataMatrix
//---------------------------------------------------------
Matrix< double > MakeBlock ( Matrix< double >         dataMatrix,
                             int                      E,
                             int                      tau,
                             std::vector<std::string> columnNames,
                             bool                     verbose ) {

    if ( columnNames.size() != dataMatrix.NColumns() ) {
        std::stringstream errMsg;
        errMsg << "MakeBlock: The number of columns in the dataMatrix ("
               << dataMatrix.NColumns() << ") is not equal to the number "
               << "of columns specified (" << columnNames.size() << ").\n";;
        throw std::runtime_error( errMsg.str() );
    }
    
    size_t NRows    = dataMatrix.NRows();        // number of input rows
    size_t NColOut  = dataMatrix.NColumns() * E; // number of output columns
    size_t NPartial = tau * (E-1);               // rows to shift & delete

    // temporary matrix to hold the embedded (shifted) data
    Matrix< double > shiftMatrix( NRows, NColOut );

    // Create embedded matrix column names X(t-0) X(t-1)...
    std::vector< std::string > newColumnNames( NColOut );
    size_t newCol_i = 0;
    for ( size_t col = 0; col < columnNames.size(); col ++ ) {
        for ( size_t e = 0; e < E; e++ ) {
            std::stringstream ss;
            ss << columnNames[ col ] << "(t-" << e << ")";
            newColumnNames[ newCol_i ] = ss.str();
            newCol_i++;
        }
    }

    // Ouput matrix with tau * E-1 fewer rows
    Matrix< double > embedding( NRows - NPartial, NColOut, newColumnNames );

    // slice indices for each column of original & shifted data
    std::slice slice_i = std::slice (0, NRows, 1);

    // to keep track of where to insert column in new matrix
    size_t colCount = 0;

    // shift column data and write to temporary matrix
    for ( size_t col = 0; col < dataMatrix.NColumns(); col++ ) {
        // for each embedding dimension
        for ( size_t e = 0; e < E; e++ ) {

            std::valarray< double > column = dataMatrix.Column( col );
            
            std::valarray< double > tmp = column.shift( -e * tau )[slice_i];

            // These will be deleted, but set to NAN for completeness
            tmp[ std::slice( 0, e * tau, 1 ) ] = NAN;

            shiftMatrix.WriteColumn( colCount, tmp );
            colCount++;
        }
    }

    // Delete rows with partial data
    slice_i = std::slice ( NPartial, NRows - NPartial, 1 );

    // Write shifted columns to the output embedding Matrix
    for ( size_t i = 0; i < NColOut; i++ ) {
        std::valarray< double > tmp = shiftMatrix.Column( i )[ slice_i ];
        embedding.WriteColumn( i, tmp );
    }
    
    return embedding;
}

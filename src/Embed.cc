
#include "Embed.h"

///---------------------------------------------------------
// Function to embed Matrix columns in E dimensions.
// if dataMatrix is NULL, read the data from path/dataFile
// side effects:        truncates the array by tau * (E-1) rows 
//                      to remove nan values (partial rows)
// @param dataMatrix:   Matrix with data columns
// @param E:            embedding dimension
// @param tau:          time step delay
// @param columns:      column names or indices to embed
// @return:             Matrix with embedded data
//---------------------------------------------------------
Matrix< double > Embed ( //Matrix< double > dataMatrix,
                         std::string      path,
                         std::string      dataFile,
                         int              E,
                         int              tau,
                         std::string      columns,
                         bool             verbose ) {
    
    // Pass in Parameters instead of create internally?
    // This will convert columns into vectors of string or size_t
    Parameters param = Parameters( Method::Simplex, path, dataFile, "",
                                   "1 1", "1 1", E, 0, 0, tau, 0,
                                   columns, "", false, verbose );

    // Load dataMatrix with columnd from path/file
    // JP This is possibly redundant file I/O
    DataFrame df = DataFrame( param.path, param.dataFile );
    Matrix< double > dataMatrix;
    size_t NColumns;
    
    if ( param.columnNames.size() ) {
        NColumns = param.columnNames.size();
        dataMatrix = df.MatrixColumnNames( param.columnNames );
    }
    else if ( param.columnIndex.size() ) {
        NColumns = param.columnIndex.size();
        dataMatrix = df.DataMatrix().MatrixColumns( param.columnIndex );
    }
    else {
        throw std::runtime_error( "Embed(): columnNames and columnIndex "
                                  " are empty.\n" );
    }
    
    size_t NRows    = dataMatrix.NRows();  // number of input rows
    size_t NColOut  = NColumns * E;        // number of output columns
    size_t NPartial = tau * (E-1);         // rows to shift & delete

    // temporary matrix to hold the embedded (shifted) data
    Matrix< double > shiftMatrix( NRows, NColOut );

    // Ouput matrix with tau * E-1 fewer rows
    Matrix< double > embedding( NRows - NPartial, NColOut );

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
    
    for ( size_t i = 0; i < NColOut; i++ ) {
        std::valarray< double > tmp = shiftMatrix.Column( i )[ slice_i ];
        embedding.WriteColumn( i, tmp );
    }
    
    return embedding;
}

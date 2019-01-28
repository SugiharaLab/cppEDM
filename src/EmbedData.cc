///---------------------------------------------------------
// File that just contains the EmbedData method and 
// a helper method  in shifting an array.
// Only should be included in DataFrame.
//---------------------------------------------------------

#include "Common.h"
#include "Matrix.h"


///---------------------------------------------------------
// method to embed the target columns in E dimensions.
// side effects:        truncates the array by tau amount to 
//                      avoid nan values (partial rows)
// @param E:            the embedding dimensions
// @param tau:          the time step delay amount
// @param targetCols:   array indices to use as target cols
// @param matrixData:   the Matrix to get the data cols from
// @return:             the Matrix with embedded data
//---------------------------------------------------------
Matrix< double > EmbedData (int E, int tau, std::vector< int > targetCols, 
        Matrix< double > matrixData) {
    //the number of rows we have to get rid since will be partial data
    size_t NPartials = tau * E;
    //new matrix dimensions with removed partial data
    //note: make sure redm uses E-1 new cols instead of E to ensure same functionality
    size_t NCols = matrixData.NColumns() + targetCols.size()*(E-1);
    size_t NRows = matrixData.NRows() - NPartials;

    //create new matrix to hold the embedded data
    Matrix< double > embeddedMatrix(NRows, NCols);

    //the slicing for each col
    std::slice sliceIndices = std::slice (NPartials, NRows, 1);

    //trim existing cols to remove partial data and append to matrix
    for (size_t colIdx = 0; colIdx < matrixData.NColumns(); colIdx++) {
        std::valarray< double > trimmed = 
            matrixData.column(colIdx)[ sliceIndices ];
        embeddedMatrix.writeColumn (colIdx, trimmed);
    }

    //to keep track of where we should insert into the new matrix
    size_t colCount = matrixData.NColumns();

    //shift target cols and append to matrix
    for (int targetCol : targetCols) {
        //for each embedding dimension
        for (int embedIdx = 1; embedIdx <= E; embedIdx++) {

            std::valarray< double > tmp = 
                matrixData.column(targetCol).shift(embedIdx*tau) [sliceIndices];
            
            embeddedMatrix.writeColumn (colCount, tmp);
            colCount++;
        }
    } 
    
    return embeddedMatrix;
}

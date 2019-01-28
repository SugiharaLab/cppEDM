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
    //the number of rows to get rid of since will there be partial data
    //note: make sure redm uses E-1 new cols per target instead of E to ensure same functionality
    size_t NPartials = tau * (E-1);
    //new matrix dimensions with removed partial data
    size_t NCols = targetCols.size() * E;
    size_t NRows = matrixData.NRows() - NPartials;

    //create new matrix to hold the embedded data
    Matrix< double > embeddedMatrix(NRows, NCols);

    //the slicing for each col to remove partial data rows
    std::slice sliceIndices = std::slice (0, NRows, 1);

    //to keep track of where we should insert into the new matrix
    size_t colCount = 0;

    //shift target cols and append to matrix
    for (size_t targetCol : targetCols) {
        //for each embedding dimension
        for (size_t embedIdx = 0; embedIdx < E; embedIdx++) {

            std::valarray< double > tmp = 
                matrixData.Column(targetCol).shift(embedIdx*tau)[sliceIndices];
                //matrixData.Column(targetCol)[sliceIndices];

            embeddedMatrix.WriteColumn (colCount, tmp);
            colCount++;

        }
    } 
    
    return embeddedMatrix;
}

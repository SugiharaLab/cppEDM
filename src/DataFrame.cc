/* contains the actual method implementations for the dataframe class */
#include "DataFrame.h"
#include "Data.cc"

#define MAX_PRINT_IDX 50

/* constructor for the dataframe
 * @param filepath: the path to the csv file to get data from
 * @param validFile: set to 0 if not valid file
 * */
DataFrame::DataFrame (const std::string filepath, const std::string fileName)
{
    //csv data to setup container and columns
    NamedData csvOutput = ReadData (filepath, fileName);
    container = SetupContainer (csvOutput);
    //setup columns * same order as in setup container so parallel to matrix
    for ( NamedData::iterator iterate = csvOutput.begin(); 
            iterate != csvOutput.end(); iterate++ ) {
        colNames.push_back( iterate->first );
    }
}

/* method to setup the data container
 * @param filepath: path to the dir csv file located in
 * @param fileName: actual name of the csv file to get
 * @return: the setup underlying container
 * */
Matrix< double > DataFrame::SetupContainer (NamedData csvOutput) {
    //for easier reference in matrix construction
    const std::size_t numRows = csvOutput.begin()->second.size();
    const std::size_t numCols = csvOutput.size();

    //setup matrix data
    Matrix<double> returnContainer = Matrix<double> (numRows, numCols);

    //transfer each vector into the matrix
    for ( NamedData::iterator iterate = csvOutput.begin(); 
            iterate != csvOutput.end(); iterate++ ) {
        size_t colIdx = std::distance(csvOutput.begin(), iterate);

        for (size_t rowIdx = 0; rowIdx < numRows; rowIdx++) {
            returnContainer(rowIdx, colIdx) = iterate->second[rowIdx];
        }
    }

    return returnContainer;
}

/* method to return the num columns/vectors in the dataframe
 * @return: the num columns 
 * */
std::size_t DataFrame::NumColumns() {
    return container.NRows();
}

/* method to return the num rows
 * takes the first col (order not preserved though since map)
 * @return: num rows
 * */
std::size_t DataFrame::NumRows() {
    //get the num rows in the first val of map col
    return container.NRows();
}

/* method to get the names of the columns as a vec
 * @return: vector with string names of the cols
 * */
std::vector<std::string> DataFrame::ColumnNames () {
    return colNames;
}

/* method to print the dataframe
 * @param os: the stream to print to
 * @return: the stream passed in
 * */
std::ostream& operator<< (std::ostream& os, DataFrame& df){

    //print info about the dataframe
    os << "DataFrame of shape - rows:"<<df.NumRows()
        <<" cols:"<<df.NumColumns()<<std::endl;
    os << "still needa format so equal spacing"<<std::endl<<std::flush;
    //print names of vectors
    for (std::size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++)
        os << df.ColumnNames().at(colIdx) << " ";
        
    os << std::endl;
    //print vec data up to 50 points
    for (std::size_t rowIdx = 0; rowIdx < df.NumRows() && rowIdx < MAX_PRINT_IDX; rowIdx++) {
        //print data points from each col
        for (std::size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++)
            os << df(rowIdx,colIdx)<<" ";
        os << std::endl;
    }

    return os;
}

/* method to access a column from the dataframe by int col/row idx
 * @param colIdx: the index to access from the dataframe 
 * @return: the NumericVector column
 * */
double & DataFrame::operator() (std::size_t rowIdx, std::size_t colIdx) {
    //handle out of bounds exception - maybe we should include this checking in matrix instead
    if (colIdx >= NumColumns())
        throw BadColIdx();
    if (rowIdx >= NumRows())
        throw BadRowIdx();
    return container(rowIdx,colIdx);
}

/* method to access a column from the dataframe by string index
 * @param colName: the name of the column to access
 * @return: the NumericVector column
 * */
double & DataFrame::operator() (std::size_t rowIdx, std::string colName) {
    //the int position of the col
    int colPos;
    //get position to check (other operator will check validity)
    std::vector< std::string >::iterator colIterate = 
        std::find(colNames.begin(), colNames.end(), colName);

    colPos = std::distance(colNames.begin(), colIterate);
    return (*this)(colPos,rowIdx);
}

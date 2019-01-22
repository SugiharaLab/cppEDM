/* contains the actual method implementations for the dataframe class */
#include "DataFrame.h"
#include "ReadData.cc"

#define MAX_PRINT_IDX 50

/* constructor for the dataframe
 * @param filepath: the path to the csv file to get data from
 * @param validFile: set to 0 if not valid file
 * */
DataFrame::DataFrame (const std::string filepath, int& validFile) {
    //to hold the output from the csv file
    NamedData csvOutput;
    //setup container map and check for bad file
    if ( !ReadData(filepath, csvOutput) )
        validFile = 0;
    //for easier reference in converting
    rowSize = csvOutput.begin()->second.size();
    int valArrSize = rowSize * csvOutput.size();

    container = std::valarray< double >(valArrSize);
    std::map<std::string,std::vector<double> >::iterator iterate = csvOutput.begin();
    //iterate over the map's vectors to create the valarray
    for ( int mapIdx = 0; iterate != csvOutput.end(); mapIdx++, iterate++ ) {
        colNames.push_back(iterate->first); 
        for ( int rowIdx = 0; rowIdx < iterate->second.size(); rowIdx++) {
            container[mapIdx*rowSize+rowIdx] = iterate->second[rowIdx];
        }
    }
}

/* method to return the num columns/vectors in the dataframe
 * @return: the num columns 
 * */
int DataFrame::NumColumns() {
    return colNames.size();
}

/* method to return the num rows
 * takes the first col (order not preserved though since map)
 * @return: num rows
 * */
int DataFrame::NumRows() {
    //get the num rows in the first val of map col
    return rowSize;
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
        <<" cols:"<<df.container.size()<<std::endl;
    os << "still needa format so equal spacing"<<std::endl<<std::flush;
    //print names of vectors
    for (int colIdx = 0; colIdx < df.colNames.size(); colIdx++)
        os << df.colNames.at(colIdx) << " ";
        
    os << std::endl;
    //print vec data up to 50 points
    for (int rowIdx = 0; rowIdx < df.NumRows() && rowIdx < MAX_PRINT_IDX; rowIdx++) {
        //print data points from each col
        for (int colIdx = 0; colIdx < df.NumColumns(); colIdx++)
            os << df(colIdx,rowIdx)<<" ";
        os << std::endl;
    }

    return os;
}

/* method to access a column from the dataframe by int col/row idx
 * @param colIdx: the index to access from the dataframe 
 * @return: the NumericVector column
 * */
double & DataFrame::operator() (int colIdx, int rowIdx) {
    //handle out of bounds exception
    if (colIdx >= NumColumns() || colIdx < 0)
        throw BadColIdx();
    if (rowIdx >= rowSize)
        throw BadRowIdx();
    return container[colIdx*rowSize+rowIdx];
}

/* method to access a column from the dataframe by string index
 * @param colName: the name of the column to access
 * @return: the NumericVector column
 * */
double & DataFrame::operator() (std::string colName, int rowIdx) {
    //the int position of the col
    int colPos;
    //get position to check (other operator will check validity)
    std::vector< std::string >::iterator colIterate = 
        std::find(colNames.begin(), colNames.end(), colName);

    colPos = std::distance(colNames.begin(), colIterate);
    return (*this)(colPos,rowIdx);
}

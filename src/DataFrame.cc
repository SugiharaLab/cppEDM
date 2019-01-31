
// NOTE: This is redundancy between DataFrame and Matrix methods to
// access and return Matrix/data values and slices.  Should be reduced. 

#include "DataFrame.h"

//------------------------------------------------------------------
// Constructor : ReadData() into the DataFrame and it's Matrix
// @param path:      path to the csv file for LoadData()
// @param fileName   input csv file
//------------------------------------------------------------------
DataFrame::DataFrame ( const std::string path, const std::string fileName,
                       size_t maxPrint ) :
    path( path ), fileName( fileName ), maxRowPrint( maxPrint ) {
    
    csvInput = ReadData ();  // csvInput is a NamedData object
    
    // Setup column names in same order as dataMatrix
    for ( NamedData::iterator iterate = csvInput.begin(); 
          iterate != csvInput.end(); iterate++ ) {
        colNames.push_back( iterate->first );
    }
    
    dataMatrix = SetupDataMatrix ( csvInput );
}

//------------------------------------------------------------------
// method to setup the dataMatrix
// @param csvInput from ReadData()
// @return: dataMatrix
//------------------------------------------------------------------
Matrix< double > DataFrame::SetupDataMatrix ( NamedData csvInput ) {
    //for easier reference in matrix construction
    const size_t numRows = csvInput.begin()->second.size();
    const size_t numCols = csvInput.size();

    // Initialize a Matrix() object with (numRows, numCols, colNames)
    Matrix<double> dataMatrix = Matrix<double> ( numRows, numCols, colNames );

    // Transfer each data value into the matrix
    //    Another option is to use the writeColumn() Matrix method
    // csvInput is : pair< string, vector<double> >
    for ( NamedData::iterator iterate  = csvInput.begin(); 
                              iterate != csvInput.end(); iterate++ ) {
        
        size_t colIdx = std::distance( csvInput.begin(), iterate );

        for ( size_t rowIdx = 0; rowIdx < numRows; rowIdx++ ) {
            dataMatrix( rowIdx, colIdx ) = iterate->second[ rowIdx ];
        }
    }

    return dataMatrix;
}

//------------------------------------------------------------------
// Return (sub)Matrix selected by columnNames
// columnNames are converted to column indices for Matrix::MatrixColumns()
//------------------------------------------------------------------
Matrix< double > DataFrame::MatrixColumnNames( std::vector<std::string>
                                               columnNames ) {
    // vector of column indices for dataMatrix.MatrixColumns()
    std::vector<size_t> col_i_vec; 

    // Map column names to indices
    std::vector<std::string>::iterator si;
    for ( auto ci = columnNames.begin(); ci != columnNames.end(); ++ci ) {
        auto si = find( colNames.begin(), colNames.end(), *ci );
        
        if ( si != colNames.end() ) {
            col_i_vec.push_back( std::distance( colNames.begin(), si ) );
        }
    }

    // Validation
    if ( col_i_vec.size() != columnNames.size() ) {
        std::stringstream errMsg;
        errMsg << "DataFrame::MatrixColumnNames() Failed to find columns:\n[ ";
        for ( auto ci = columnNames.begin(); ci != columnNames.end(); ++ci ) {
            errMsg << *ci << " ";
        } errMsg << "]" << std::endl;
        errMsg << "in DataFrame columns:\n[ ";
        for ( auto ci = colNames.begin(); ci != colNames.end(); ++ci ) {
            errMsg << *ci << " ";
        } errMsg << "]" << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    Matrix<double> M_col = dataMatrix.MatrixColumns( col_i_vec );

    // Now insert the columnNames
    M_col.ColumnNames() = columnNames;
    M_col.BuildColumnNameIndex();

    return M_col;
}

//------------------------------------------------------------------
// Return data column selected by columnName
//------------------------------------------------------------------
std::valarray< double > DataFrame::VectorColumnName( std::string column ) {

    std::vector< std::string >::iterator ci = std::find( colNames.begin(),
                                                         colNames.end(),
                                                         column );
    if ( ci == colNames.end() ) {
        std::stringstream errMsg;
        errMsg << "DataFrame::VectorColumnName() Failed to find column: "
               << column;
        errMsg << " in DataFrame columns:\n[ ";
        for ( auto cni = colNames.begin(); cni != colNames.end(); ++cni ) {
            errMsg << *cni << " ";
        } errMsg << "]" << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    size_t col_i = std::distance( colNames.begin(), ci );

    std::valarray<double> vec = dataMatrix.Column( col_i );
    
    return vec;
}

//------------------------------------------------------------------
// method to access a value by row and column index
// @param colIdx: the index to access from the dataframe 
// @return: the data value
//------------------------------------------------------------------
double & DataFrame::operator() ( size_t rowIdx, size_t colIdx ) {
    
    if ( colIdx >= NumColumns() ) {  // JP should this be > instead of >= ?
        std::stringstream errMsg;
        errMsg << "DataFrame(r,c): column index c=" << colIdx
               << " exceeds bounds (" << NumColumns() << ")" << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    if ( rowIdx >= NumRows() ) {
        std::stringstream errMsg;
        errMsg << "DataFrame(r,c): row index r=" << rowIdx
               << " exceeds bounds (" << NumRows() << ")" << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    
    return dataMatrix( rowIdx, colIdx );
}

//------------------------------------------------------------------
// method to access a value by row index and column name
// @param colName: the name of the column to access
// @return: the data value
//------------------------------------------------------------------
double & DataFrame::operator() ( size_t rowIdx, std::string colName ) {

    int colPos;
    
    //get position to check (other operator will check validity)
    std::vector< std::string >::iterator colIterate = 
        std::find( colNames.begin(), colNames.end(), colName );

    if ( colIterate == colNames.end() ) {
        std::stringstream errMsg;
        errMsg << "DataFrame( r, colName ): Column " << colName
               << " was not found in data from file " << fileName << std::endl;
        throw std::runtime_error( errMsg.str() );
    }

    colPos = std::distance( colNames.begin(), colIterate );
    
    return (*this)( rowIdx, colPos);
}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
NamedData DataFrame::ReadData() {
    
    // Create input file stream and open file for input
    std::ifstream dataStrm( path + fileName );
    
    //make sure file access is good before reading
    if ( not dataStrm.is_open() ) {
        std::stringstream errMsg;
        errMsg << "ERROR: ReadData() file " << path + fileName
               << " is not open for reading." << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    if ( not dataStrm.good() ) {
        std::stringstream errMsg;
        errMsg << "ERROR: ReadData() file " << path + fileName
               << " is not ready for reading." << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    
    // Read fileName into a vector of strings, one line per string
    std::vector< std::string > dataLines;
    std::string tmp;
    
    while( getline( dataStrm, tmp ) ) {
        dataLines.push_back( tmp );
    }
    dataStrm.close();
    
#ifdef DEBUG_ALL
    std::cout << "------- ReadData() Contents of file "
              << fileName << " -------" << std::endl;
    for( std::vector< std::string >::iterator ci = dataLines.begin();
         ci != dataLines.end(); ++ci ) {
        std::cout << *ci << std::endl;
    }
#endif
    
    // Container for data names/vectors
    NamedData namedData; // pair< string, vector< double >> NamedData;
    // Container of column names in the same order as in csv file
    std::vector< std::string > colNames;

    // Check first line to see if it's only numeric digits, or a header
    bool onlyDigits = true;
    std::vector<std::string> firstLineWords = SplitString( dataLines[0] );
    
    for ( auto si = firstLineWords.begin(); si != firstLineWords.end(); ++si ){

        onlyDigits = OnlyDigits( *si );
        
        if ( not onlyDigits ) { break; }
    }
    if ( onlyDigits ) {
        // create named columns with generic col names: COL_PREFIX
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++ ) {
            colNames.push_back( COL_PREFIX + std::to_string(colIdx) );
        }
    }
    else {
        // get named columns from header line
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++) {
            colNames.push_back( firstLineWords[colIdx] );  
        }
        // remove header line from read in lines so only numerical after
        dataLines.erase( dataLines.begin() );
    }
    
    // setup each col in namedData with new vec to insert numerical data to
    for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
        NamedData::value_type colPair ( colNames[colIdx],
                                        std::vector<double>());
        namedData.push_back ( colPair );
    }
    
    // Process each line in dataLines to fill in data vectors
    for ( size_t lineIdx = 0; lineIdx < dataLines.size(); lineIdx++ ) {
        std::vector<std::string> words = SplitString( dataLines[ lineIdx ] );
        for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
            namedData[ colIdx ].second.push_back( std::stod( words[colIdx] ) );
        } 
    }
        
#ifdef DEBUG_ALL
    std::cout << "------- ReadData() data from "
              << fileName << " -------" << std::endl;
    for ( auto ci = namedData.begin(); ci != namedData.end(); ++ci ) {
        std::cout << ci->first << " ";
    } std::cout << std::endl;

    for ( size_t row = 0; row < (namedData.begin()->second).size(); row++ ) {
        for ( auto ci = namedData.begin(); ci != namedData.end(); ++ci ) {
            std::vector< double > vec = ci->second;
            std::cout << vec[ row ] << " ";
        } std::cout << std::endl;
    }
#endif
    
    return namedData;
}

//------------------------------------------------------------------
//  method to read the DataFrame data to file
//  @param outputFilePath: path to the file to write to
//  @param outputFileName: filename to write to 
//  @return: true if write was good, false if bad
//------------------------------------------------------------------
void DataFrame::WriteData(std::string outputFilePath, std::string outputFileName) {
    //to hold the lines to print to the output file
    std::vector< std::string > fileLines;

    //tmp string to hold one line at a time
    std::stringstream lineStr;

    //save col names line
    for (auto & colName : colNames) {
        lineStr << colName << " ";
    }

    //set and empty
    fileLines.push_back( lineStr.str() );
    lineStr.str(std::string());

    //for more efficient access inside inner loop below
    size_t nCols = dataMatrix.NColumns();

    //iterate through all numerical data to print
    for (size_t rowIdx = 0; rowIdx < dataMatrix.NRows(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < nCols; colIdx++) {

            lineStr << dataMatrix (rowIdx, colIdx);

            if (colIdx != nCols-1) {
                lineStr << ", ";
            }
        }

        //set and empty
        fileLines.push_back( lineStr.str() );
        lineStr.str(std::string());
    }

    //write contents to file
    std::ofstream outputFile(outputFilePath + outputFileName);
    if (outputFile.is_open()) {

        std::copy(fileLines.begin(), fileLines.end(),
                std::ostream_iterator<std::string>(outputFile,"\n"));

        outputFile.close();
    }

    //bad write if got to here
    else {
        std::stringstream errMsg;
        errMsg << "DataFrame::WriteData(): bad file permissions: "
            << outputFilePath + outputFileName << ". \n";
        throw std::runtime_error( errMsg.str() );
    }

}

//------------------------------------------------------------------
// method to print the dataframe
//  @param os: the stream to print to
//  @return: the stream passed in
//------------------------------------------------------------------
std::ostream& operator<< ( std::ostream& os, DataFrame& df )
{
    // print info about the dataframe
    os << "DataFrame: -----------------------------------\n";
    os << df.NumRows() << " rows, " << df.NumColumns() << " columns.\n";
    
    os << "----------------------------------------------" << std::endl;
    // print names of columns
    for ( size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++ ) {
        os << df.ColumnNames().at(colIdx) << " ";
    } os << std::endl;

    os << "---------------- First " << df.MaxRowPrint()
       << " rows ---------------\n";
        
    // print vec data up to maxRowPrint points
    for ( size_t rowIdx = 0;
          rowIdx < df.NumRows() and rowIdx < df.MaxRowPrint(); rowIdx++ ) {
        
        // print data points from each col
        for ( size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++ ) {
            os << df(rowIdx, colIdx) << " ";
        }
        os << std::endl;
    }

    os << "----------------------------------------------" << std::endl;
    
    return os;
}

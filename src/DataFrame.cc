
#include "DataFrame.h"

//------------------------------------------------------------------
// Constructor : LoadData() into the DataFrame and it's Matrix
// @param path:      path to the csv file for LoadData()
// @param fileName   input csv file
//------------------------------------------------------------------
DataFrame::DataFrame (const std::string path, const std::string fileName) :
    path( path ), fileName( fileName ) {
    
    //csv data to setup container and columns
    csvInput  = ReadData ();
    container = SetupContainer ( csvInput );
    
    //setup columns * same order as in setup container so parallel to matrix
    for ( NamedData::iterator iterate = csvInput.begin(); 
          iterate != csvInput.end(); iterate++ ) {
        colNames.push_back( iterate->first );
    }
}

//------------------------------------------------------------------
// method to setup the data container
// @param csvInput: path to the dir csv file located in
// @return: the setup underlying container
//------------------------------------------------------------------
Matrix< double > DataFrame::SetupContainer ( NamedData csvInput ) {
    //for easier reference in matrix construction
    const size_t numRows = csvInput.begin()->second.size();
    const size_t numCols = csvInput.size();

    // Initialize a Matrix() object with (numRows, numCols)
    Matrix<double> returnContainer = Matrix<double> (numRows, numCols);

    // Transfer each vector into the matrix
    // Another option is to use the writeColumn() Matrix method
    for ( NamedData::iterator iterate  = csvInput.begin(); 
                              iterate != csvInput.end(); iterate++ ) {
        
        size_t colIdx = std::distance( csvInput.begin(), iterate );

        for ( size_t rowIdx = 0; rowIdx < numRows; rowIdx++ ) {
            returnContainer( rowIdx, colIdx ) = iterate->second[ rowIdx ];
        }
    }

    return returnContainer;
}

//------------------------------------------------------------------
//------------------------------------------------------------------
size_t DataFrame::NumColumns() {
    return container.NColumns();
}

//------------------------------------------------------------------
//------------------------------------------------------------------
size_t DataFrame::NumRows() {
    return container.NRows();
}

//------------------------------------------------------------------
//------------------------------------------------------------------
std::vector<std::string> DataFrame::ColumnNames () {
    return colNames;
}

//------------------------------------------------------------------
// method to print the dataframe
//  @param os: the stream to print to
//  @return: the stream passed in
//------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, DataFrame& df){

    // print info about the dataframe
    os << "DataFrame: " << df.NumRows() << " rows, "
       << df.NumColumns() << " columns." << std::endl;

    os << "-----------------------------------------------" << std::endl;
    
    // print names of vectors
    for (size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++) {
        os << df.ColumnNames().at(colIdx) << " ";
    } os << std::endl;

    os << "---------------- First 50 rows ----------------" << std::endl;
        
    // print vec data up to 50 points
    for (size_t rowIdx = 0; rowIdx < df.NumRows() && rowIdx < MAX_PRINT_IDX;
         rowIdx++) {
        //print data points from each col
        for (size_t colIdx = 0; colIdx < df.NumColumns(); colIdx++) {
            os << df(rowIdx,colIdx)<<" ";
        }
        os << std::endl;
    }

    return os;
}

//------------------------------------------------------------------
// method to access a column from the dataframe by int col/row idx
// @param colIdx: the index to access from the dataframe 
// @return: the NumericVector column
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
    
    return container( rowIdx, colIdx );
}

//------------------------------------------------------------------
// method to access a column from the dataframe by string index
// @param colName: the name of the column to access
// @return: the NumericVector column
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
    
    return (*this)( colPos, rowIdx );
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
    NamedData data; // map< string, vector< double >> NamedData;
    // Container of column names in the same order as in csv file
    std::vector< std::string > colNames;

    // Check first line to see if it's only numeric digits, or a header
    std::vector<std::string> firstLineWords = SplitString( dataLines[0] );
    bool isHeader = false;
    for ( auto si = firstLineWords.begin(); si != firstLineWords.end(); ++si ){
        const char *word = si->c_str(); // Crap: isdigit() is a C function...
        for ( size_t i = 0; i < si->size(); i++ ) {
            if ( not isdigit( word[i] ) ) { isHeader = true; break; }
        }
        if ( isHeader ) { break; }
    }
    if ( isHeader ) {
        // setup named columns
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++) {
            colNames.push_back( firstLineWords[colIdx] );  
        }
        // remove header line from read in lines so only numerical after
        dataLines.erase(dataLines.begin());
    }
    // setup named columns with generic col names: COL_PREFIX?
    else {
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++ ) {
            colNames.push_back( COL_PREFIX + std::to_string(colIdx) );
        } 
    }
    // setup each col in output data with new vec to insert numerical data to
    for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
        NamedData::value_type colPair ( colNames[colIdx],
                                        std::vector<double>(dataLines.size()));
        data.insert ( colPair );
    }
    
    // Process each line in dataLines to fill in data vectors
    for ( size_t lineIdx = 0; lineIdx < dataLines.size(); lineIdx++ ) {
        std::vector<std::string> words = SplitString( dataLines[ lineIdx ] );
        
        for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
            std::string colName = colNames[ colIdx ];

            data[ colName ][ lineIdx ] = std::stod( words[colIdx] );
        } 
    }
        
#ifdef DEBUG_ALL
    std::cout << "------- ReadData() data from "
              << fileName << " -------" << std::endl;
    for ( auto ci = data.begin(); ci != data.end(); ++ci ) {
        std::cout << ci->first << " ";
    } std::cout << std::endl;

    for ( size_t row = 0; row < (data.begin()->second).size(); row++ ) {
        for ( auto ci = data.begin(); ci != data.end(); ++ci ) {
            std::vector< double > vec = ci->second;
            std::cout << vec[ row ] << " ";
        } std::cout << std::endl;
    }
#endif
    
    return data;
}

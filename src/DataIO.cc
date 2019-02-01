
#include "DataIO.h"

//------------------------------------------------------------------
// Constructor : ReadData() into the DataIO and it's DataFrame
// @param path:      path to the csv file for LoadData()
// @param fileName   input csv file
//------------------------------------------------------------------
DataIO::DataIO ( const std::string path, const std::string fileName,
                       size_t maxPrint ) :
    path( path ), fileName( fileName ), maxRowPrint( maxPrint ) {
    
    csvInput = ReadData ();  // csvInput is a NamedData object
    
    dataFrame = SetupDataFrame ( csvInput );
}

//------------------------------------------------------------------
// method to setup the dataFrame
// @param csvInput from ReadData()
// @return: dataFrame
//------------------------------------------------------------------
DataFrame< double > DataIO::SetupDataFrame ( NamedData csvInput ) {

    // Setup column names in same order as dataFrame
    std::vector< std::string > colNames;
    for ( NamedData::iterator iterate = csvInput.begin(); 
          iterate != csvInput.end(); iterate++ ) {
        colNames.push_back( iterate->first );
    }
    
    // for easier reference in data frame construction
    const size_t numRows = csvInput.begin()->second.size();
    const size_t numCols = csvInput.size();

    // Initialize a DataFrame() object with (numRows, numCols, colNames)
    DataFrame<double> dataFrame = DataFrame<double>(numRows, numCols, colNames);

    // Transfer each data value into the data frame
    //    Another option is to use the writeColumn() DataFrame method
    // csvInput is : pair< string, vector<double> >
    for ( NamedData::iterator iterate  = csvInput.begin(); 
                              iterate != csvInput.end(); iterate++ ) {
        
        size_t colIdx = std::distance( csvInput.begin(), iterate );

        for ( size_t rowIdx = 0; rowIdx < numRows; rowIdx++ ) {
            dataFrame( rowIdx, colIdx ) = iterate->second[ rowIdx ];
        }
    }

    return dataFrame;
}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
NamedData DataIO::ReadData() {
    
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
        // create named columns with generic col names: V1, V2...
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++ ) {
            colNames.push_back( "V" + std::to_string(colIdx) );
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
//  method to read the DataIO data to file
//  @param outputFilePath: path to the file to write to
//  @param outputFileName: filename to write to 
//  @return: true if write was good, false if bad
//------------------------------------------------------------------
void DataIO::WriteData(std::string outputFilePath, std::string outputFileName) {
    //to hold the lines to print to the output file
    std::vector< std::string > fileLines;

    //tmp string to hold one line at a time
    std::stringstream lineStr;

    //for more efficient access inside inner loop below
    size_t nCols = DFrame().NColumns();

    //save col names line
    for (size_t colIdx = 0; colIdx < nCols; colIdx++) {
        lineStr << DFrame().ColumnNames()[ colIdx ];

        if ( colIdx != nCols - 1 ) {
            lineStr << ",";
        }
    }

    //set and empty
    fileLines.push_back( lineStr.str() );
    lineStr.str(std::string()); // would lineStr.flush() do the same?

    //iterate through all numerical data to print
    for (size_t rowIdx = 0; rowIdx < DFrame().NRows(); rowIdx++) {
        for (size_t colIdx = 0; colIdx < nCols; colIdx++) {

            lineStr << DFrame() (rowIdx, colIdx);

            if ( colIdx != nCols - 1 ) {
                lineStr << ",";
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
        errMsg << "DataIO::WriteData(): bad file permissions: "
            << outputFilePath + outputFileName << ". \n";
        throw std::runtime_error( errMsg.str() );
    }

}

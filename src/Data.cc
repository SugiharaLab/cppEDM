#include <fstream>

#include "Data.h"
#define DEBUG_ALL 1

#define VALID_DOUBLE_CHARS "1234567890.,"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
NamedData ReadData( std::string path,
                    std::string fileName ) {
    
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
    NamedData data;
    // Container specifically for the column names 
    std::vector< std::string > colNames;

    // Check first line for named columns
    std::vector<std::string> firstLineWords = SplitString( dataLines[0] );
    if (firstLineWords.front().find_first_not_of(VALID_DOUBLE_CHARS)
            !=std::string::npos) {
        //setup named columns
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++) {
            colNames.push_back(firstLineWords[colIdx]);    
        } 
        //remove header line from read in lines so only numerical after
        dataLines.erase(dataLines.begin());
    }
    //setup named columns with generic col names
    else {
        for ( size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++) {
            colNames.push_back(COL_PREFIX+std::to_string(colIdx));    
        } 
    }
    //setup each col in output data with new vec to insert numerical data to
    for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++) {
        NamedData::value_type colPair (colNames[colIdx], std::vector< double >());
        data.insert (colPair);
    }
    
    // Process each line in dataLines to setup namedData
    for ( size_t lineIdx = 0; lineIdx < dataLines.size(); lineIdx++ ) {
        std::vector<std::string> numericalWords = SplitString( dataLines[lineIdx] );
        
        for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
            double rawNum = std::stod(numericalWords[colIdx]);
            //i think having to lookup in the map for every insertion is too expensive - todo:change
            data.find(colNames[colIdx])->second.push_back(rawNum);
        } 
    }
        
    return data;
}

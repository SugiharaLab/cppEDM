#include <fstream>

#include "Data.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
NamedData ReadData( std::string path = "./",
                    std::string fileName = "Data.csv" ) {
    
    // Create input file stream and open file for input
    std::ifstream dataStrm( path + fileName );

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

#ifdef DEBUG
    std::cout << "------- ReadData() Contents of file "
         << fileName << " -------" << std::endl;
    for( std::vector< std::string >::iterator ci = dataLines.begin();
         ci != dataLines.end(); ++ci ) {
        std::cout << *ci << std::endl;
    }
#endif

    NamedData data;
    
    return data;
}

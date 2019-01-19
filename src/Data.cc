#include <fstream>

#include "Data.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
NamedData ReadData( string path = "./", string fileName = "Data.csv" ) {
    
    // Create input file stream and open file for input
    ifstream dataStrm( path + fileName );

    if ( not dataStrm.is_open() ) {
        stringstream errMsg;
        errMsg << "ERROR: ReadData() file " << path + fileName
               << " is not open for reading." << endl;
        throw runtime_error( errMsg.str() );
    }
    if ( not dataStrm.good() ) {
        stringstream errMsg;
        errMsg << "ERROR: ReadData() file " << path + fileName
               << " is not ready for reading." << endl;
        throw runtime_error( errMsg.str() );
    }

    // Read fileName into a vector of strings, one line per string
    vector< string > dataLines;
    string tmp;

    while( getline( dataStrm, tmp ) ) {
        dataLines.push_back( tmp );
    }
    dataStrm.close();

#ifdef DEBUG
    cout << "------- ReadData() Contents of file "
         << fileName << " -------" << endl;
    for( vector< string >::iterator ci = dataLines.begin();
         ci != dataLines.end(); ++ci ) {
        cout << *ci << endl;
    }
#endif

    NamedData data;
    
    return data;
}

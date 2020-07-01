
// g++ dataFrame.cc -o dataFrame -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -llapack -pthread

#include "API.h"
#include "DataFrame.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile   = "../data/block_3sp.csv";
    std::string columns    = "x_t";
    std::string fileOut    = "out.csv";
    bool        verbose    = false;     // 'y' = true
    
    if ( argc > 1 ) { dataFile  = argv[1]; }
    if ( argc > 2 ) { columns   = argv[2]; }
    if ( argc > 3 ) { fileOut   = argv[3]; }
    if ( argc > 4 ){ verbose    = ( *argv[4] == 'y' ? true : false );  }

    if ( verbose ) {
        std::cout << dataFile << " cols " << columns << std::endl;
    }
    
    try {
        //----------------------------------------------------------
        //----------------------------------------------------------

        DataFrame< double > DF;

        DF = DataFrame < double >( "",         // pathIn
                                   dataFile ); // dataFile

        if ( verbose ) {
            DF.MaxRowPrint() = DF.NRows();
            std::cout << DF;
        }

        size_t rowi[] = { 0,2,4,6,8,10 };
        std::vector< size_t > rows( rowi, rowi + sizeof(rowi)/sizeof(size_t) );
        DataFrame < double > DFrows = DF.DataFrameFromRowIndex( rows );
        std::cout << DFrows;

        size_t coli[] = { 1 };
        std::vector< size_t > cols( coli, coli + sizeof(coli)/sizeof(size_t) );
        DataFrame < double > DFcols = DF.DataFrameFromColumnIndex( cols );
        std::cout << DFcols;

        DataFrame < double > DFrowscols =
            DFrows.DataFrameFromColumnIndex( cols );
        std::cout << DFrowscols;
        
        if ( fileOut.size() ) {
            DF.WriteData( "", fileOut );
        }
    }
    
    catch ( const std::exception& e ) {
 	std::cout << "Exception caught in main:\n";
        std::cout << e.what() << std::endl;
	return -1;
    }
    catch (...) {
 	std::cout << "Unknown exception caught in main.\n";
	return -1;
    }

    std::cout << "normal termination\n";
    
    return 0;
}

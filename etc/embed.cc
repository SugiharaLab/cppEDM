
// g++ embed.cc -o embed -g -I../src -L../lib -lstdc++ -lEDM -llapack -pthread

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile      = "../data/block_3sp.csv";
    std::string columns       = "x_t";
    std::string fileOut       = "embed_out.csv";
    int         E             = 3;
    int         tau           = -1;
    bool        deletePartial = false;  // 'y' = true
    bool        verbose       = false;  // 'y' = true

    if ( argc > 1 ) { dataFile  = argv[1]; }
    if ( argc > 2 ) { columns   = argv[2]; }
    if ( argc > 3 ) { fileOut   = argv[3]; }
    if ( argc > 4 ) { std::stringstream ss( argv[4] ); ss >> E;           }    
    if ( argc > 5 ) { std::stringstream ss( argv[5] ); ss >> tau;         }
    if ( argc > 6 ) { deletePartial = ( *argv[6] == 'y' ? true : false ); }
    if ( argc > 7 ) { verbose       = ( *argv[7] == 'y' ? true : false ); }

    if ( verbose ) {
        std::cout << dataFile << " cols " << columns
                  << " E " << E << " tau " << tau << std::endl;
    }

    try {
        //----------------------------------------------------------
        //----------------------------------------------------------

        DataFrame<double> dataFrame;

        dataFrame = Embed(  "",         // pathIn
                            dataFile,   // dataFile
                            E,          // E
                            tau,        // tau
                            columns,    // columns
                            verbose );  // verbose

        if ( verbose ) {
            dataFrame.MaxRowPrint() = dataFrame.NRows();
            std::cout << dataFrame;
        }

        if ( fileOut.size() ) {
            dataFrame.WriteData( "", fileOut );
        }

        // MakeBlock
        DataFrame<double> data( "../data/", "circle.csv" );
        
        DataFrame<double> embedding = MakeBlock( std::ref( data ), E, tau,
                                                 data.ColumnNames(),
                                                 deletePartial );
        if ( verbose ) {
            embedding.MaxRowPrint() = embedding.NRows();
            std::cout << embedding;
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

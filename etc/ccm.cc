
// g++ ccm.cc -o ccm -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -llapack -lpthread

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile    = "../data/sardine_anchovy_sst.csv";
    std::string columns     = "anchovy";
    std::string target      = "np_sst";
    std::string fileOut     = "ccm-out.csv";
    int         E           = 3;
    int         Tp          = 0;
    std::string libSizes    = "10 75 5";
    int         sample      = 0;
    bool        random      = false;     // 'y' = true
    bool        replacement = false;     // 'y' = true
    bool        verbose     = false;     // 'y' = true
    
    if ( argc > 1 ) { dataFile   = argv[1]; }
    if ( argc > 2 ) { columns    = argv[2]; }
    if ( argc > 3 ) { target     = argv[3]; }
    if ( argc > 4 ) { fileOut    = argv[4]; }
    if ( argc > 5 ) { std::stringstream ss( argv[5] ); ss >> E;          }
    if ( argc > 6 ) { std::stringstream ss( argv[6] ); ss >> Tp;         }
    if ( argc > 7 ) { libSizes   = argv[7];                              }
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> sample;     }
    if ( argc > 9  ){ random      = ( *argv[9]  == 'y' ? true : false ); }
    if ( argc > 10 ){ replacement = ( *argv[10] == 'y' ? true : false ); }
    if ( argc > 11 ){ verbose     = ( *argv[11] == 'y' ? true : false ); }

    if ( verbose ) {
        std::cout << dataFile << " " << columns << " " << target << " "
                  << E << " " << Tp << " " << libSizes << std::endl;
    }
    
    try {        
        CCMValues ccmOut = CCM( "./",         // pathIn,
                                dataFile,
                                "./",         // pathOut,
                                fileOut,      // predictFile,
                                E,
                                Tp,
                                0,            // knn,
                                -1,           // tau,
                                columns,
                                target,
                                libSizes,     // libSizes_str,
                                sample,
                                random,
                                replacement,
                                0,            // seed,
                                false,        // includeData
                                verbose );

        DataFrame<double> dataFrame = ccmOut.AllLibStats;
        dataFrame.MaxRowPrint() = dataFrame.NRows();

        if ( verbose ) {
            std::cout << dataFrame;
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

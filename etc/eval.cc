
// g++ eval.cc -o eval -I../src -L../lib -lstdc++ -lEDM -lpthread -llapack

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string method        = "E"; // E, Tp or theta
    std::string dataFile      = "../data/block_3sp.csv";
    std::string lib           = "1 100";
    std::string pred          = "101 180";
    std::string columns       = "x_t";
    std::string target        = "x_t";
    std::string fileOut       = "out.csv";
    int         E             = 3;
    int         Tp            = 1;
    std::string theta         = "";
    bool        embedded      = false;   // 'y' = true
    bool        verbose       = false;   // 'y' = true
    bool        const_pred    = false;   // 'y' = true
    int         exclusionR    = 0;
    int         nThreads      = 1;

    if ( argc > 1 ) { method     = argv[1]; }
    if ( argc > 2 ) { dataFile   = argv[2]; }
    if ( argc > 3 ) { lib        = argv[3]; }
    if ( argc > 4 ) { pred       = argv[4]; }
    if ( argc > 5 ) { columns    = argv[5]; }
    if ( argc > 6 ) { target     = argv[6]; }
    if ( argc > 7 ) { fileOut    = argv[7]; }
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> E;            }
    if ( argc > 9 ) { std::stringstream ss( argv[9] ); ss >> Tp;           }
    if ( argc > 10 ){ theta         = argv[10];                            }
    if ( argc > 11 ){ embedded      = ( *argv[11] == 'y' ? true : false ); }
    if ( argc > 12 ){ verbose       = ( *argv[12] == 'y' ? true : false ); }
    if ( argc > 13 ){ const_pred    = ( *argv[13] == 'y' ? true : false ); }
    if ( argc > 14 ){ std::stringstream ss( argv[14] ); ss >> exclusionR;  }
    if ( argc > 15 ){ std::stringstream ss( argv[15] ); ss >> nThreads;    }

    if ( verbose ) {
        std::cout << method << " " << dataFile << " lib: " << lib
                  << " pred: " << pred << " " << columns << " "
                  << target << " E: " << E << " Tp: " << Tp
                  << " theta: " << theta << std::endl;
    }

    try {
        if ( method.compare( "E" ) == 0 ) {

            // EmbedDimension
            DataFrame< double > EMBD = 
                EmbedDimension( "",            // pathIn
                                dataFile,      // dataFile
                                "./",          // pathOut
                                fileOut,       // predictFile
                                lib,           // lib
                                pred,          // pred
                                10,            // maxE       <<<<< CONSTANT
                                Tp,            // Tp
                                -1,            // tau        <<<<< CONSTANT
                                exclusionR,    // exclusionRadius
                                columns,       // colNames
                                target,        // targetName
                                embedded,      // embedded
                                verbose,       // verbose
                                std::vector<bool>(), // validLib
                                nThreads );    // nThreads

            std::cout << "EmbedDimension:\n";
            if ( verbose ) {
                std::cout << EMBD;
            }
        }

        else if ( method.compare( "Tp" ) == 0 ) {

            // PredictInterval
            DataFrame< double > PD = 
                PredictInterval( "",            // pathIn
                                 dataFile,      // dataFile
                                 "./",          // pathOut
                                 fileOut,       // predictFile
                                 lib,           // lib
                                 pred,          // pred
                                 10,            // maxTp      <<<<< CONSTANT
                                 E,             // E
                                 -1,            // tau        <<<<< CONSTANT
                                 exclusionR,    // exclusionRadius
                                 columns,       // colNames
                                 target,        // targetName
                                 embedded,      // embedded
                                 verbose,       // verbose
                                 std::vector<bool>(), // validLib
                                 nThreads );    // nThreads

            std::cout << "PredictInterval:\n";
            if ( verbose ) {
                std::cout << PD;
            }
        }

        else if ( method.compare( "theta" ) == 0 ) {

            // PredictNonlinear
            DataFrame< double > NL = 
                PredictNonlinear( "",            // pathIn
                                  dataFile,      // dataFile,
                                  "./",          // pathOut
                                  fileOut,       // predictFile
                                  lib,           // lib
                                  pred,          // pred
                                  theta,         // theta
                                  E,             // E
                                  Tp,            // Tp
                                  0,             // knn
                                  -1,            // tau         <<<<< CONSTANT
                                  exclusionR,    // exclusionRadius
                                  columns,       // colNames
                                  target,        // targetName
                                  embedded,      // embedded
                                  verbose,       // verbose
                                  std::vector<bool>(), // validLib
                                  nThreads );    // nThreads

            NL.MaxRowPrint() = 15;
            std::cout << "PredictNonlinear:\n";
            if ( verbose ) {
                std::cout << NL;
            }
        }

        else {
            std::cout << "No method found" << std::endl;
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

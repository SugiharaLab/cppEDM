
// g++ mview.cc -o mview -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -lpthread -llapack

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile   = "../data/block_3sp.csv";
    std::string lib        = "1 100";
    std::string pred       = "101 195";
    std::string columns    = "x_t y_t z_t";
    std::string target     = "x_t";
    std::string fileOut    = "mview-out.csv";
    int         D          = 0;
    int         E          = 3;
    int         Tp         = 1;
    int         multiview  = 0;
    int         exclusionR = 0;
    bool        trainLib   = false; // 'y' = true
    bool        verbose    = true;  // 'y' = true
    int         nThreads   = 1;
    
    if ( argc > 1 ) { dataFile   = argv[1]; }
    if ( argc > 2 ) { lib        = argv[2]; }
    if ( argc > 3 ) { pred       = argv[3]; }
    if ( argc > 4 ) { columns    = argv[4]; }
    if ( argc > 5 ) { target     = argv[5]; }
    if ( argc > 6 ) { fileOut    = argv[6]; }
    if ( argc > 7 ) { std::stringstream ss( argv[7] ); ss >> D;          }    
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> E;          }    
    if ( argc > 9 ) { std::stringstream ss( argv[9] ); ss >> Tp;         }
    if ( argc > 10) { std::stringstream ss( argv[10]); ss >> multiview;  }
    if ( argc > 11) { std::stringstream ss( argv[11]); ss >> exclusionR; }
    if ( argc > 12) { trainLib = ( *argv[12] == 'y' ? true : false );    }
    if ( argc > 13) { verbose  = ( *argv[13] == 'y' ? true : false );    }
    if ( argc > 14) { std::stringstream ss( argv[14] ); ss >> nThreads;  }

    if ( verbose ) {
        std::cout << dataFile << "  lib: " << lib << "  pred: " << pred
                  << "  columns: " << columns << "  target: " << target
                  << "  D: " << D << "  E: " << E << "  Tp: " << Tp
                  << "  multiview: " << multiview
                  << "  exclusionR: " << exclusionR
                  << "  trainLib: " << trainLib << std::endl;
    }
    
    try {
        //----------------------------------------------------------
        //
        //----------------------------------------------------------
        MultiviewValues MV =
            Multiview( "",        // pathIn
                       dataFile,  // dataFile
                       "./",      // pathOut
                       fileOut,   // predictFile
                       lib,       // lib
                       pred,      // pred
                       D,         // D
                       E,         // E
                       Tp,        // Tp
                       0,         // knn        <<<<< CONSTANT
                       -1,        // tau        <<<<< CONSTANT
                       columns,   // columns
                       target,    // target,
                       multiview, // multiview
                       exclusionR,// exclusionRadius
                       trainLib,  // trainLib
                       false,     // verbose,
                       nThreads );// nThreads
        
        DataFrame< double > MVPredictions = MV.Predictions;

        VectorError vemv =
            ComputeError( MVPredictions.VectorColumnName( "Observations" ),
                          MVPredictions.VectorColumnName( "Predictions"  ) );

        std::cout << "Multiview()" << dataFile << "\nrho " << vemv.rho
                  << "  MAE "  << vemv.MAE
                  << "  RMSE " << vemv.RMSE << std::endl;

        if ( verbose ) {
            // Table of columns, names, rho, MAE RMSE
            std::cout << std::endl;
            std::vector< std::string > table = MV.ComboRhoTable;
            for ( size_t row = 0; row < table.size(); row++ ) {
                std::cout << table[ row ] << std::endl;
            }
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


// g++ edm.cc -o edm -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -llapack -pthread

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile   = "../data/block_3sp.csv";
    std::string lib        = "1 100";
    std::string pred       = "101 195";
    std::string columns    = "x_t";
    std::string target     = "x_t";
    std::string fileOut    = "out.csv";
    int         E          = 3;
    int         Tp         = 1;
    int         tau        = -1; 
    std::string method     = "simplex"; // or smap
    int         theta      = 2;
    bool        embedded   = false;     // 'y' = true
    bool        verbose    = false;     // 'y' = true
    bool        const_pred = false;     // 'y' = true
    int         exclusionR = 0;
    
    if ( argc > 1 ) { dataFile   = argv[1]; }
    if ( argc > 2 ) { lib        = argv[2]; }
    if ( argc > 3 ) { pred       = argv[3]; }
    if ( argc > 4 ) { columns    = argv[4]; }
    if ( argc > 5 ) { target     = argv[5]; }
    if ( argc > 6 ) { fileOut    = argv[6]; }
    if ( argc > 7 ) { std::stringstream ss( argv[7] ); ss >> E;           }    
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> Tp;          }
    if ( argc > 9 ) { std::stringstream ss( argv[9] ); ss >> tau;         }
    if ( argc > 10 ) { method     = argv[10]; }    
    if ( argc > 11 ){ std::stringstream ss( argv[11] ); ss >> theta;      }
    if ( argc > 12 ){ embedded   = ( *argv[12] == 'y' ? true : false );   }
    if ( argc > 13 ){ verbose    = ( *argv[13] == 'y' ? true : false );   }
    if ( argc > 14 ){ const_pred = ( *argv[14] == 'y' ? true : false );   }
    if ( argc > 15 ){ std::stringstream ss( argv[15] ); ss >> exclusionR; }

    if ( verbose ) {
        std::cout << method << " " << dataFile << " lib " << lib << " pred "
                  << pred << " cols " << columns << " target " << target
                  << " E " << E << " Tp " << Tp << " tau " << tau
                  << " theta " << theta << std::endl;
    }
    
    try {
        
        //----------------------------------------------------------
        //----------------------------------------------------------
        bool simplex = true;
        if ( method.find( "smap" ) != std::string::npos ) { simplex = false; }

        DataFrame<double> dataFrame;
        DataFrame<double> coef;
        
        if ( simplex ) {
            dataFrame = Simplex( "",         // pathIn
                                 dataFile,   // dataFile
                                 "./",       // pathOut
                                 fileOut,    // predictFile
                                 lib,        // lib
                                 pred,       // pred
                                 E,          // E
                                 Tp,         // Tp
                                 0,          // knn             <<<<< CONSTANT
                                 tau,        // tau
                                 exclusionR, // exclusionRadius
                                 columns,    // columns
                                 target,     // target
                                 embedded,   // embedded
                                 const_pred, // const_predict
                                 verbose );  // verbose
        }
        else {
            SMapValues SM = SMap( "",               // pathIn
                                  dataFile,         // dataFile
                                  "./",             // pathOut
                                  fileOut,          // predictFile
                                  lib,              // lib
                                  pred,             // pred
                                  E,                // E
                                  Tp,               // Tp
                                  0,                // knn      <<<<< CONSTANT
                                  tau,              // tau
                                  theta,            // theta
                                  exclusionR,       // exclusionRadius
                                  columns,          // columns
                                  target,           // target
                                  "smap_coeff.csv", // smapFile
                                  "",               // derivatives
                                  embedded,         // embedded
                                  const_pred,       // const_predict
                                  verbose );        // verbose

            dataFrame = SM.predictions;
            coef      = SM.coefficients;
        }

        VectorError ve = ComputeError(
            dataFrame.VectorColumnName( "Observations" ),
            dataFrame.VectorColumnName( "Predictions"  ) );

        std::cout << method << " on " << dataFile << ":\n";
        std::cout << "rho   " << ve.rho << "  RMSE   " << ve.RMSE
                  << "  MAE   " << ve.MAE << std::endl;

        if ( const_pred ) {
            ve = ComputeError(
                dataFrame.VectorColumnName( "Observations" ),
                dataFrame.VectorColumnName( "Const_Predictions"  ) );
            
            std::cout << "rho_c " << ve.rho << "  RMSE_c " << ve.RMSE
                      << "  MAE_c " << ve.MAE << std::endl << std::endl;
        }

        if ( verbose ) {
            dataFrame.MaxRowPrint() = dataFrame.NRows();
            std::cout << dataFrame;
            if ( not simplex ) {
                std::cout << coef;
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

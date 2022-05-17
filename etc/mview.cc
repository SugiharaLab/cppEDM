
// g++ mview.cc -o mview -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -lpthread -llapack

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile      = "../data/block_3sp.csv";
    std::string lib           = "1 100";
    std::string pred          = "101 195";
    std::string columns       = "x_t y_t z_t";
    std::string target        = "x_t";
    std::string fileOut       = "mview-out.csv";
    int         D             = 0;
    int         E             = 3;
    int         Tp            = 1;
    int         multiview     = 0;
    int         exclusionR    = 0;
    bool        trainLib      = false; // 'y' = true
    bool        excludeTarget = false; // 'y' = true
    bool        verbose       = true;  // 'y' = true
    bool        parameterList = false; // 'y' = true
    int         nThreads      = 1;

    if ( argc > 1 ) { dataFile            = argv[1]; }
    if ( argc > 2 ) { lib                 = argv[2]; }
    if ( argc > 3 ) { pred                = argv[3]; }
    if ( argc > 4 ) { columns             = argv[4]; }
    if ( argc > 5 ) { target              = argv[5]; }
    if ( argc > 6 ) { fileOut             = argv[6]; }
    if ( argc > 7 ) { std::stringstream ss( argv[7] ); ss >> D;            }    
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> E;            }    
    if ( argc > 9 ) { std::stringstream ss( argv[9] ); ss >> Tp;           }
    if ( argc > 10) { std::stringstream ss( argv[10]); ss >> multiview;    }
    if ( argc > 11) { std::stringstream ss( argv[11]); ss >> exclusionR;   }
    if ( argc > 12) { trainLib      = ( *argv[12] == 'y' ? true : false ); }
    if ( argc > 13) { excludeTarget = ( *argv[13] == 'y' ? true : false ); }
    if ( argc > 14) { verbose       = ( *argv[14] == 'y' ? true : false ); }
    if ( argc > 15) { parameterList = ( *argv[15] == 'y' ? true : false ); }
    if ( argc > 16) { std::stringstream ss( argv[16] ); ss >> nThreads;    }

    if ( verbose ) {
        std::cout << dataFile << "  lib: " << lib << "  pred: " << pred
                  << "  columns: " << columns << "  target: " << target
                  << "  D: " << D << "  E: " << E << "  Tp: " << Tp
                  << "  multiview: " << multiview
                  << "  exclusionR: " << exclusionR
                  << "  trainLib: " << trainLib
                  << "  excludeTarget: " << excludeTarget
                  << "  parameterList: " << parameterList << std::endl;
    }

    try {
        //----------------------------------------------------------
        //
        //----------------------------------------------------------
        MultiviewValues MV =
            Multiview( "",            // pathIn
                       dataFile,      // dataFile
                       "./",          // pathOut
                       fileOut,       // predictFile
                       lib,           // lib
                       pred,          // pred
                       D,             // D
                       E,             // E
                       Tp,            // Tp
                       0,             // knn        <<<<< CONSTANT
                       -1,            // tau        <<<<< CONSTANT
                       columns,       // columns
                       target,        // target,
                       multiview,     // multiview
                       exclusionR,    // exclusionRadius
                       trainLib,      // trainLib
                       excludeTarget, // excludeTarget
                       parameterList, // parameterList
                       false,         // verbose
                       nThreads );    // nThreads

        DataFrame< double > MVPredictions = MV.Predictions;

        VectorError vemv =
            ComputeError( MVPredictions.VectorColumnName( "Observations" ),
                          MVPredictions.VectorColumnName( "Predictions"  ) );

        std::cout << "Multiview()" << dataFile << "\nrho " << vemv.rho
                  << "  MAE "  << vemv.MAE
                  << "  RMSE " << vemv.RMSE << std::endl;

        if ( verbose ) {
            // Columns, rho, MAE RMSE
            std::cout << MV.ComboRho;
            std::cout << std::endl;

            std::map< std::string, std::vector< std::string > >
                colNames = MV.ColumnNames;

            for ( auto cit = colNames.begin(); cit != colNames.end(); ++cit ){
                std::cout << cit->first << ": ";
                std::vector< std::string > columnNames = cit->second;
                for ( auto nit  = columnNames.begin();
                      nit != columnNames.end(); ++nit ) {
                    std::cout << *nit << " ";
                }
                std::cout << std::endl;
            }   std::cout << std::endl;
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

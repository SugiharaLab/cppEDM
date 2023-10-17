
// g++ edmNan.cc -o edmNan -g -I../src -L../lib -lstdc++ -lEDM -llapack -pthread

#include "API.h"

//---------------------------------------------------------------------
// nanRows & nanCols start at 1. Internally 0-offset as are lib, pred
// if numNan > 0, randomly select numNan rows for nan, else use nanRows
//---------------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile        = "../data/block_3sp.csv";
    std::string lib             = "1 100";
    std::string pred            = "101 195";
    std::string columns         = "x_t";
    std::string target          = "x_t";
    std::string fileOut         = "out.csv";
    int         E               = 3;
    int         Tp              = 1;
    int         tau             = -1;
    int         knn             = 0;
    int         exclusionRadius = 0;
    std::string method          = "simplex"; // or smap
    int         theta           = 2;
    bool        embedded        = false;     // 'y' = true
    bool        ignoreNan       = true;      // 'n' = false
    bool        verbose         = false;     // 'y' = true
    bool        const_pred      = false;     // 'y' = true
    bool        parameterList   = false;     // 'y' = true
    bool        generateLibrary = false;     // 'y' = true
    int         generateSteps   = 0;
    int         numNan          = 0; // if >0 random sample else nanRows
    std::string nanRows         = "190 191 195 197"; // Origin 1 not 0
    std::string nanCols         = "  0   0   3   3";
    
    if ( argc > 1 ) { dataFile  = argv[1]; }
    if ( argc > 2 ) { lib       = argv[2]; }
    if ( argc > 3 ) { pred      = argv[3]; }
    if ( argc > 4 ) { columns   = argv[4]; }
    if ( argc > 5 ) { target    = argv[5]; }
    if ( argc > 6 ) { fileOut   = argv[6]; }
    if ( argc > 7 ) { std::stringstream ss( argv[7] ); ss >> E;   }
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> Tp;  }
    if ( argc > 9 ) { std::stringstream ss( argv[9] ); ss >> tau; }
    if ( argc > 10) { std::stringstream ss( argv[10]); ss >> knn; }
    if ( argc > 11) { std::stringstream ss( argv[11]); ss >> exclusionRadius; }
    if ( argc > 12 ){ method     = argv[12]; }
    if ( argc > 13 ){ std::stringstream ss( argv[13] ); ss >> theta;         }
    if ( argc > 14 ){ embedded        = ( *argv[14] == 'y' ? true : false ); }
    if ( argc > 15 ){ ignoreNan       = ( *argv[15] == 'y' ? true : false ); }
    if ( argc > 16 ){ verbose         = ( *argv[16] == 'y' ? true : false ); }
    if ( argc > 17 ){ const_pred      = ( *argv[17] == 'y' ? true : false ); }
    if ( argc > 18 ){ parameterList   = ( *argv[18] == 'y' ? true : false ); }
    if ( argc > 19 ){ generateLibrary = ( *argv[19] == 'y' ? true : false ); }
    if ( argc > 20 ){ std::stringstream ss( argv[20] ); ss >> generateSteps; }
    if ( argc > 21) { std::stringstream ss( argv[21] ); ss >> numNan;        }
    if ( argc > 22 ){ nanRows         = argv[22]; }
    if ( argc > 23 ){ nanCols         = argv[23]; }

    if ( verbose ) {
        std::cout << method << " " << dataFile << " lib " << lib << " pred "
                  << pred << " cols " << columns << " target " << target
                  << " E " << E << " Tp " << Tp << " tau " << tau
                  << " knn " << knn << " exclusionRadius " << exclusionRadius
                  << " theta " << theta << " embedded " << embedded
                  << " ignoreNan " << ignoreNan
                  << " generateSteps " << generateSteps
                  << " generateLibrary " << generateLibrary
                  << " numNan " << numNan
                  << " nanRows " << nanRows << " nanCols " << nanCols
                  << std::endl << std::flush;
    }

    try {
        DataFrame< double > DF;
        DF = DataFrame < double >( "",         // pathIn
                                   dataFile ); // dataFile

        std::vector< size_t > nanRow;
        std::vector< size_t > nanCol;

        if ( numNan > 0 ) { // Generate nanRows into nanRow vector
            // Random generator : set seed
            typedef std::chrono::high_resolution_clock CCMclock;
            CCMclock::time_point beginning = CCMclock::now();
            CCMclock::duration   duration  = CCMclock::now() - beginning;
            int seed = duration.count();
            // Index limits for RNG
            int RNGStart = 0;
            int RNGStop  = (int) DF.NRows() - 1;
            // Init generator
            std::default_random_engine DefaultRandomEngine( seed );
            // Create RNG sampler into allLibRows for this libSize
            std::uniform_int_distribution< size_t >
                distribution( RNGStart, RNGStop );

            std::unordered_set< size_t > nanRowSet; // no duplicates
            for ( size_t i = 0; i < numNan; i++ ) {
                size_t v = distribution( DefaultRandomEngine );
                nanRowSet.insert( v );
            }
            // Copy to nanRow
            nanRow.insert( nanRow.begin(), nanRowSet.begin(), nanRowSet.end() );
            
            // Get nanCols : only use the first value for all rows
            std::stringstream ss( nanCols );
            size_t nanColVal;
            ss >> nanColVal;
            nanCol.insert( nanCol.begin(), nanRow.size(), nanColVal - 1 );
        }
        else { // user specified nanRow, nanCol
            std::vector< std::string >
                nanRow_str = SplitString( nanRows, " ,\t\n" );
            
            std::vector< std::string >
                nanCol_str = SplitString( nanCols, " ,\t\n" );
            
            size_t val;
            for (auto str : nanRow_str) {
                std::stringstream ss(str);
                ss >> val;
                nanRow.push_back( val - 1 ); // 1 origin, not 0
            }
            for (auto str : nanCol_str) {
                std::stringstream ss(str);
                ss >> val;
                nanCol.push_back( val - 1 ); // 1 origin, not 0
            }
        }

        // Set nan in DF
        for ( size_t i = 0; i < nanRow.size(); i++ ) {
            DF( nanRow[i], nanCol[i] ) = nan("dataFrame");
            std::cout << "DF(" << nanRow[i] << ","
                      << nanCol[i] << ") = nan\n" << std::flush;
        }

        if ( verbose ) {
            std::cout << DF;
        }

        //----------------------------------------------------------
        //----------------------------------------------------------
        bool simplex = true;
        if ( method.find( "smap" ) != std::string::npos ) { simplex = false; }

        DataFrame<double> dataFrame;
        DataFrame<double> coef;
        std::map< std::string, std::string > parameterMap;

        if ( simplex ) {
            SimplexValues S = Simplex( std::ref( DF ),  // dataFrame
                                       "./",            // pathOut
                                       fileOut,         // predictFile
                                       lib,             // lib
                                       pred,            // pred
                                       E,               // E
                                       Tp,              // Tp
                                       knn,             // knn
                                       tau,             // tau
                                       exclusionRadius, // exclusionRadius
                                       columns,         // columns
                                       target,          // target
                                       embedded,        // embedded
                                       const_pred,      // const_predict
                                       verbose,         // verbose
                                       std::vector<bool>(), // validLib
                                       generateSteps,   // generateSteps
                                       generateLibrary, // generateLibrary
                                       parameterList ); // parameterList

            dataFrame    = S.predictions;
            parameterMap = S.parameterMap;
        }
        else {
            SMapValues SM = SMap( std::ref( DF ),   // dataFrame
                                  "./",             // pathOut
                                  fileOut,          // predictFile
                                  lib,              // lib
                                  pred,             // pred
                                  E,                // E
                                  Tp,               // Tp
                                  knn,              // knn
                                  tau,              // tau
                                  theta,            // theta
                                  exclusionRadius,  // exclusionRadius
                                  columns,          // columns
                                  target,           // target
                                  "smap_coeff.csv", // smapCoefFile
                                  "smap_sv.csv",    // smapSVFile
                                  embedded,         // embedded
                                  const_pred,       // const_predict
                                  verbose,          // verbose
                                  std::vector<bool>(), // validLib
                                  ignoreNan,        // ignoreNan
                                  generateSteps,    // generateSteps
                                  generateLibrary,  // generateLibrary
                                  parameterList );  // parameterList

            dataFrame    = SM.predictions;
            coef         = SM.coefficients;
            parameterMap = SM.parameterMap;
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

        if ( parameterList ) {
            std::cout << "----------- Parameters -------------" << std::endl;
            for ( auto pi =  parameterMap.begin();
                       pi != parameterMap.end(); ++pi ) {
                std::cout << pi->first << " : " << pi->second << std::endl;
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

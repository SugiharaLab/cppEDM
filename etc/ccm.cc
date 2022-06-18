
// g++ ccm.cc -o ccm -std=c++11 -g -I../src -L../lib -lstdc++ -lEDM -llapack -lpthread

#include "API.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile        = "../data/sardine_anchovy_sst.csv";
    std::string columns         = "anchovy";
    std::string target          = "np_sst";
    std::string fileOut         = "ccm-out.csv";
    int         E               = 3;
    int         Tp              = 0;
    int         tau             = -1;
    int         exclusionRadius = 0;
    std::string libSizes        = "10 75 5";
    int         sample          = 0;
    bool        embedded        = false;     // 'y' = true
    bool        random          = false;     // 'y' = true
    bool        replacement     = false;     // 'y' = true
    bool        verbose         = false;     // 'y' = true
    bool        parameterList   = false;     // 'y' = true

    if ( argc > 1 ) { dataFile   = argv[1]; }
    if ( argc > 2 ) { columns    = argv[2]; }
    if ( argc > 3 ) { target     = argv[3]; }
    if ( argc > 4 ) { fileOut    = argv[4]; }
    if ( argc > 5 ) { std::stringstream ss( argv[5] ); ss >> E;            }
    if ( argc > 6 ) { std::stringstream ss( argv[6] ); ss >> Tp;           }
    if ( argc > 7 ) { std::stringstream ss( argv[7] ); ss >> tau;          }
    if ( argc > 8 ) { std::stringstream ss( argv[8] ); ss >> exclusionRadius; }
    if ( argc > 9 ) { libSizes   = argv[9];                                }
    if ( argc > 10 ){ std::stringstream ss( argv[10] ); ss >> sample;      }
    if ( argc > 11 ){ embedded      = ( *argv[11] == 'y' ? true : false ); }
    if ( argc > 12 ){ random        = ( *argv[12] == 'y' ? true : false ); }
    if ( argc > 13 ){ replacement   = ( *argv[13] == 'y' ? true : false ); }
    if ( argc > 14 ){ verbose       = ( *argv[14] == 'y' ? true : false ); }
    if ( argc > 15 ){ parameterList = ( *argv[15] == 'y' ? true : false ); }

    if ( verbose ) {
        std::cout << dataFile << " " << columns << " " << target
                  << " E " << E << " Tp " << Tp << " tau " << tau
                  << " exclusionRadius " << exclusionRadius 
                  << " libSizes " << libSizes
                  << " sample " << sample << " embedded " << embedded
                  << " random " << random << " replacement " << replacement
                  << " parameterList " << parameterList << std::endl;
    }

    try {        
        CCMValues ccmOut = CCM( "./",         // pathIn,
                                dataFile,
                                "./",         // pathOut,
                                fileOut,      // predictFile,
                                E,
                                Tp,
                                0,            // knn,
                                tau,
                                exclusionRadius,
                                columns,
                                target,
                                libSizes,
                                sample,
                                random,
                                replacement,
                                0,            // seed,
                                embedded,
                                true,         // includeData
                                parameterList,
                                verbose );

        DataFrame<double> dataFrame = ccmOut.AllLibStats;
        dataFrame.MaxRowPrint() = dataFrame.NRows();

        DataFrame<double> predictStats = ccmOut.CrossMap1.PredictStats;
        predictStats.MaxRowPrint() = 5;

        std::forward_list< DataFrame< double > > predList =
            ccmOut.CrossMap1.Predictions;
        DataFrame<double> predResult = *predList.begin();
        predResult.MaxRowPrint() = 5; // predResult.NRows();

        if ( verbose ) {
            std::cout << dataFrame;
            //std::cout << predictStats;
            //std::cout << predResult;

            predictStats.WriteData( "./", "ccm-stats-out.csv" );
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

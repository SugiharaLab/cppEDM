
// g++ TestCCM.cc -o TestCCM -std=c++11 -g -DDEBUG -DCCM_THREADED -I../src -L../lib -lstdc++ -lEDM -lpthread

#include <chrono>

#include "Common.h"

using namespace std;
using namespace std::chrono;

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {

        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        //----------------------------------------------------------
        DataFrame< double > CCMD = 
            CCM( "../../ZebraFish/", "Fish_150a_Hypo.csv",
                 "./", "HypoOutCCM.csv",
                 3, 0, 0, 1, "D_16672", "D_6488", "1500 1500 10", 1,
                 false, 0, false, true );
        
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>( t2 - t1 ).count();

        cout << "CCM() execution time: " << duration << " (uS)\n";
        cout << CCMD << endl;
    
        t1 = high_resolution_clock::now();
        DataFrame<double> dataFrame1 = 
            Simplex( "../../ZebraFish/", "Fish_150a_Hypo.csv", "./",
                     "HypoOutSmplx.csv",
                     "1 1500", "1 1500", 3, 0, 0, 1,
                     "D_16672", "D_6488", false, false );
        
        DataFrame<double> dataFrame2 = 
            Simplex( "../../ZebraFish/", "Fish_150a_Hypo.csv", "./",
                     "HypoOutSmplx.csv",
                     "1 1500", "1 1500", 3, 0, 0, 1,
                     "D_6488", "D_16672", false, false );
        
        t2 = high_resolution_clock::now();

        duration = duration_cast<microseconds>( t2 -t1 ).count();

        cout << "Simplex() execution time: " << duration << " (uS)\n";
        //dataFrameEmbed.MaxRowPrint() = 12; // Set number of rows to print
        //std::cout << dataFrameEmbed;

        VectorError ve1 = ComputeError(
            dataFrame1.VectorColumnName( "Observations" ),
            dataFrame1.VectorColumnName( "Predictions"  ) );

        VectorError ve2 = ComputeError(
            dataFrame2.VectorColumnName( "Observations" ),
            dataFrame2.VectorColumnName( "Predictions"  ) );

        cout << "Simplex: D_16672 : D_6488 ";
        cout << "rho " << ve1.rho << "  RMSE " << ve1.RMSE
             << "  MAE " << ve1.MAE << endl;
        cout << "Simplex: D_6488 : D_16672 ";
        cout << "rho " << ve2.rho << "  RMSE " << ve2.RMSE
             << "  MAE " << ve2.MAE << endl;
    }
    catch ( const std::exception& e ) {
 	cout << "Exception caught in main:\n";
        cout << e.what() << endl;
	return -1;
    }
    catch (...) {
 	cout << "Unknown exception caught in main.\n";
	return -1;
    }

    cout << "normal termination\n";
    
    return 0;
}

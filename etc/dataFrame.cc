
// g++ dataFrame.cc -o dataFrame -g -I../src -L../lib -lstdc++ -lEDM

#include "API.h"
#include "DataFrame.h"

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {

    std::string dataFile   = "../data/block_3sp.csv";
    std::string columns    = "x_t y_t z_t";
    std::string fileOut    = "";
    bool        verbose    = false;     // 'y' = true

    if ( argc > 1 ) { dataFile  = argv[1]; }
    if ( argc > 2 ) { columns   = argv[2]; }
    if ( argc > 3 ) { fileOut   = argv[3]; }
    if ( argc > 4 ){ verbose    = ( *argv[4] == 'y' ? true : false );  }

    if ( verbose ) {
        std::cout << dataFile << " columns " << columns << std::endl;
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

        // Data Frame From Row Index
        size_t rowi[] = { 0,2,4,6,8,10 };
        std::vector< size_t > rows( rowi, rowi + sizeof(rowi)/sizeof(size_t) );
        DataFrame < double > DFrows = DF.DataFrameFromRowIndex( rows );
        std::cout << ">>>>>>> DataFrameFromRowIndex\n";
        std::cout << DFrows;

        // Data Frame From Column Index
        size_t coli[] = { 1, 4, 7 };
        std::vector< size_t > cols( coli, coli + sizeof(coli)/sizeof(size_t) );
        DataFrame < double > DFcols = DF.DataFrameFromColumnIndex( cols );
        std::cout << ">>>>>>> DataFrameFromColumnIndex\n";
        std::cout << DFcols;

        // Data Frame From Column Names
        std::vector<std::string> colNames = SplitString( columns, " \t\n" );
        DataFrame < double > DFrowscols =
            DFrows.DataFrameFromColumnNames( colNames );
        std::cout << ">>>>>>> DataFrameFromColumnNames\n";
        std::cout << DFrowscols;

        // Data Frame Remove Nan Rows
        // Set nan in DF
        DF( 185, 0 ) = nan("dataFrame"); // x_t
        DF( 188, 5 ) = nan("dataFrame"); // y_t-2
        DF( 189, 3 ) = nan("dataFrame"); // y_t
        DF( 190, 0 ) = nan("dataFrame"); // x_t
        DF( 193, 2 ) = nan("dataFrame"); // x_t-2
        DF( 195, 4 ) = nan("dataFrame"); // y_t-1

        bool nanFound = DF.NanRows( colNames ); // set DF validRows
        DataFrame < double > DFnan = DF.DataFrameRemoveNanRows();
        std::cout << ">>>>>>> DataFrameRemoveNanRows\n";
        std::cout << "---------- nan rows: ";
        for ( auto n : DF.NanRows() ) { std::cout << n << " "; }
        std::cout << std::endl;
        std::cout << DFnan;

        // Copy constructor
        DFrows( 0, 0 ) = -999.0;
        DataFrame< double > DFC = DFrows;
        DFrows( 1, 0 ) = -888.0;
        std::cout << ">>>>>>> DFRows copy\n";
        std::cout << DFC;
        

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

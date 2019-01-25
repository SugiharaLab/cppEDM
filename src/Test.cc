
#include "DataFrame.h"
#include "Neighbors.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {

        DataFrame df = DataFrame( "../data/", "block_3sp.csv" );

        const Matrix< double > &M = df.matrix();

        std::vector< std::string > colNames = df.ColumnNames();
        std::valarray<double>      row0     = M.row( 0 );

        std::cout << "Data Frame from block_3sp.csv has "
                  << df.NumColumns() << "(" << M.NColumns() << ") and "
                  << df.NumRows() << "(" << M.NRows() << ") rows." << std::endl;
        std::cout << "Column Names: ";
        for ( auto ci = colNames.begin(); ci != colNames.end(); ++ci ) {
            std::cout << *ci << ", ";
        } std::cout << std::endl;
        std::cout << "Row 0: ";
        for ( auto ri = begin(row0); ri != end(row0); ++ri ) {
            std::cout << *ri << ", ";
        } std::cout << std::endl;
        
        
        //--------------------------------------------------
        // Default parameters
        //--------------------------------------------------
        Parameters parameters = Parameters();
        parameters.verbose    = true;
        parameters.embedded   = true;
        parameters.knn        = 4;
        parameters.Tp         = 1;
        parameters.library    = { 1,   150 };
        parameters.prediction = { 181, 198 };

        parameters.Validate();
        std::cout << parameters;
        
        // Call FindNeighbors()
        //--------------------------------------------------
        const Parameters &params = parameters;
        Neighbors neighbors = FindNeighbors( M, params );

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

//----------------------------------------------------------------
//----------------------------------------------------------------
void TestCode() {
    
        //--------------------------------------------------
        // Matrix 6 rows 3 columns
        //--------------------------------------------------
        Matrix<double> M( 6, 3 );
        for ( size_t i = 0; i < 6; i++ ) {
            for ( size_t j = 0; j < 3; j++ ) {
                M( i, j ) = i + j * 2;
            }
        }
        
        std::valarray<double> A( 99, 3 );
        M.writeRow( 0, A );
        std::valarray<double> B( -1, 6 );
        M.writeColumn( 1, B );

        // test Matrix.row() & column()
        std::cout << "Matrix:" << std::endl;
        for ( size_t i = 0; i < 6; i++ ) {
            std::cout << "Row " << i << " : ";
            for ( size_t j = 0; j < 3; j++ ) {
                std::cout << M(i,j) << " ";
            } std::cout << std::endl;
        }   
        std::cout << "Matrix rows():" << std::endl;
        for ( size_t i = 0; i < 6; i++ ) {
            std::valarray<double> row = M.row( i );
            std::cout << "Row " << i << " : ";
            for ( size_t j = 0; j < row.size(); j++ ) {
                std::cout << row[j] << " ";
            } std::cout << std::endl;
        }
        std::cout << "Matrix cols():" << std::endl;
        for ( size_t i = 0; i < 3; i++ ) {
            std::valarray<double> col = M.column( i );
            std::cout << "Column " << i << " : ";
            for ( size_t j = 0; j < col.size(); j++ ) {
                std::cout << col[j] << " ";
            } std::cout << std::endl;
        }

}

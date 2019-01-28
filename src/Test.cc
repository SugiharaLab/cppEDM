
#include "DataFrame.h"
#include "Neighbors.h"

//----------------------------------------------------------------
// Intended to execute tests to validate the code.
// For now, it's open for use as a library test.
//----------------------------------------------------------------
int main( int argc, char *argv[] ) {
    
    try {
        //--------------------------------------------------
        // 
        //--------------------------------------------------
        Matrix<double> dataMatrix = 
        Simplex( "../data/", "block_3sp.csv", "", "1 100", "190 198",
                 3, 1, 0, 1, "x_t y_t z_t", "x_t", true, true );

        std::cout << dataMatrix;
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
        M.WriteRow( 0, A );
        std::valarray<double> B( -1, 6 );
        M.WriteColumn( 1, B );

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
            std::valarray<double> row = M.Row( i );
            std::cout << "Row " << i << " : ";
            for ( size_t j = 0; j < row.size(); j++ ) {
                std::cout << row[j] << " ";
            } std::cout << std::endl;
        }
        std::cout << "Matrix cols():" << std::endl;
        for ( size_t i = 0; i < 3; i++ ) {
            std::valarray<double> col = M.Column( i );
            std::cout << "Column " << i << " : ";
            for ( size_t j = 0; j < col.size(); j++ ) {
                std::cout << col[j] << " ";
            } std::cout << std::endl;
        }

}


#include "Matrix.h"

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
template <class T>
Matrix<T>::Matrix( size_t rows, size_t columns ):
    n_rows( rows ), n_columns( columns ), elements( columns * rows ) {}

//-----------------------------------------------------------------
// Write array to row
//-----------------------------------------------------------------
template<typename T>
void Matrix<T>::writeRow( size_t row, std::valarray<T> array ) {
    size_t N = array.size();
    
    if ( N != n_columns ) {
        std::stringstream errMsg;
        errMsg << "Matrix::insertRow(): row argument must be "
               << n_columns << ". " << N << " were provided." << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    for ( size_t i = 0; i < N; i++ ) {
        (*this)( row, i ) = array[ i ];
    }
}

//-----------------------------------------------------------------
// Write array to col
//-----------------------------------------------------------------
template<typename T>
void Matrix<T>::writeColumn( size_t col, std::valarray<T> array ) {
    size_t N = array.size();
    
    if ( N != n_rows ) {
        std::stringstream errMsg;
        errMsg << "Matrix::insertColumn(): col argument must be "
               << n_rows << ". " << N << " were provided." << std::endl;
        throw std::runtime_error( errMsg.str() );
    }
    for ( size_t i = 0; i < N; i++ ) {
        (*this)( i, col ) = array[ i ];
    }
}

//-----------------------------------------------------------------
// Template classes are a nuisance
// Since we want to have a separate Matrix.cc file with methods
// for the Matrix template class, i.e. we Don't want to move all
// the methods and code for Matrix into Matrix.h, here we can
// instantiate the needed types explicitly.  Only these types will
// be supported as Matrix types. 
//-----------------------------------------------------------------
template class Matrix<int>;
template class Matrix<double>;

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>
#include <map>

//prefix for columns without header titles
#define COL_PREFIX "vec_"

// Type definitions
typedef std::map< std::string, std::vector< double > > NamedData;

// Enumerations
enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };


// Prototypes
//---------------------------------------------------------
std::vector<std::string> SplitString( std::string inString, 
                                      std::string delimeters = "," );

//---------------------------------------------------------
// Matrix class
// Data container is a single, contiguous valarray.
// Matrix access is through the () operator: (row,col).
//---------------------------------------------------------
template <class T>
class Matrix {
    std::valarray<T> elements;
    size_t           n_columns;
    size_t           n_rows;
    
public:
    Matrix( size_t rows, size_t columns ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ) {}
    Matrix () {}

    // Fortran style access operators M(row,col)
    T &operator()( size_t row, size_t column ) {
        return elements[ row * n_columns + column ];
    }
    T operator()( size_t row, size_t column ) const {
        return elements[ row * n_columns + column ];
    }
    
    size_t NColumns() const { return n_columns; }
    size_t NRows()    const { return n_rows;    }
    size_t size()     const { return n_rows * n_columns; }

    // Return column from index col
    std::valarray<T> column( size_t col ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( col, n_rows, n_columns ) ];
    }

    // Return row from index row
    std::valarray<T> row( size_t row ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( row * n_columns, n_columns, 1 ) ];
    }

    // Insert row
    void writeRow( size_t row, std::valarray<T> array ) {
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

    // Write the array to col
    void writeColumn( size_t col, std::valarray<T> array ) {
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
};

#endif

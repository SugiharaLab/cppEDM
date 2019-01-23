#ifndef MATRIX_H
#define MATRIX_H

#include <valarray>

#include "Common.h"

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
    Matrix( size_t rows, size_t columns );
    Matrix () {}

    // Operators and accessors
    
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

    // Prototypes defined in Matrix.cc
    // Write array to row or col
    void writeRow   ( size_t row, std::valarray<T> array );
    void writeColumn( size_t col, std::valarray<T> array );
};

#endif

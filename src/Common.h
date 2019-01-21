#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>
#include <map>

// Type definitions
typedef std::map< std::string, std::vector< double > > NamedData;

// Enumerations
enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Prototypes
//---------------------------------------------------------
std::vector<std::string> Tokenize( std::string message );

//---------------------------------------------------------
// Matrix class
// Data container is a single, contiguous valarray.
// Matrix access is through the () operator: (row,col).
//---------------------------------------------------------
template <class T>
class Matrix {
    std::valarray<T> elements;
    std::size_t      n_columns;
    std::size_t      n_rows;
    
public:
    Matrix( size_t rows, size_t columns ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ) {}

    // Fortran style access operators M(row,col)
    T &operator()( std::size_t row, std::size_t column ) {
        return elements[ row * n_columns + column ];
    }
    T operator()( std::size_t row, std::size_t column ) const {
        return elements[ row * n_columns + column ];
    }
    
    size_t NColumns() const { return n_columns; }
    size_t NRows()    const { return n_rows;    }
    size_t size()     const { return n_rows * n_columns; }

    // Return column from index col
    std::valarray<T> column( std::size_t col ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( col, n_rows, n_columns ) ];
    }

    // Return row from index row
    std::valarray<T> row( std::size_t row ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( row * n_columns, n_columns, 1 ) ];
    }
};

#endif

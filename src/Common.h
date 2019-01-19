#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>
#include <map>

using namespace std; // This is poor usage... should be explicit namespaces.

// type definitions
typedef map< string, vector< double > > NamedData;
typedef vector< vector< double > >      DataBlock;
typedef vector< vector< int > >         IndexBlock;
typedef valarray< valarray< double> >   ArrayBlock;

enum class Method { Simplex, SMap };
enum class DistanceMetric { Euclidean, Manhattan };

// Prototypes
//---------------------------------------------------------
vector<string> Tokenize( string message );

//---------------------------------------------------------
//---------------------------------------------------------
template <class T>
class Matrix {
    std::valarray<T> elements;
    size_t n_columns;
    size_t n_rows;
    
public:
    Matrix( size_t rows, size_t columns ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ) {}

    T &operator()( size_t row, size_t column ) {
        return elements[ row * n_columns + column ];
    }
    T operator()( size_t row, size_t column ) const {
        return elements[ row * n_columns + column ];
    }
    size_t NColumns() const { return n_columns; }
    size_t NRows()    const { return n_rows;    }
};

#endif

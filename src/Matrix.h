#ifndef MATRIX_H
#define MATRIX_H

// NOTE: This is redundancy between DataFrame and Matrix methods to
// access and return Matrix/data values and slices.  Should be reduced. 

// NOTE: This header deviates from the desired class implementation
// where *.h provides declarations, *.cc methods.  This is solely to
// accomodate the OSX XCode environment which seems unable to deal
// with c++11 standard template implemenations.
// A possible solution is to link against libc++ on OSX. See ../etc/.

#include "Common.h"

// Since #include Matrix.h is in Common.h, need forward declaration
extern std::vector<std::string> SplitString( std::string inString, 
                                             std::string delimeters );

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
    std::vector< std::string >      columnNames;
    std::map< std::string, size_t > columnNameToIndex;
    
public:
    //-----------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------
    Matrix () {}
    
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    Matrix( size_t rows, size_t columns ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ) {}
    
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    Matrix( size_t rows, size_t columns, std::string colNames ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ),
        columnNames( std::vector<std::string>(columns) )
    {
        BuildColumnNameIndex( colNames );
    }
   
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    Matrix( size_t rows, size_t columns,
            std::vector< std::string > columnNames ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ),
        columnNames( columnNames )
    {
        BuildColumnNameIndex();
    }
   
    //-----------------------------------------------------------------
    // Fortran style access operators M(row,col)
    //-----------------------------------------------------------------
    T &operator()( size_t row, size_t column ) {
        return elements[ row * n_columns + column ];
    }
    T operator()( size_t row, size_t column ) const {
        return elements[ row * n_columns + column ];
    }

    //-----------------------------------------------------------------
    // Member Accessors
    //-----------------------------------------------------------------
    size_t NColumns() const { return n_columns; }
    size_t NRows()    const { return n_rows;    }
    size_t size()     const { return n_rows * n_columns; }
    std::vector< std::string >  ColumnNames() const { return columnNames; }
    std::vector< std::string > &ColumnNames()       { return columnNames; }
    std::map< std::string, size_t > ColumnNameToIndex() const {
        return columnNameToIndex;
    }
    std::map< std::string, size_t > &ColumnNameToIndex() {
        return columnNameToIndex;
    }

    //-----------------------------------------------------------------
    // Return column from index col
    //-----------------------------------------------------------------
    std::valarray<T> Column( size_t col ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( col, n_rows, n_columns ) ];
    }

    //-----------------------------------------------------------------
    // Return row from index row
    //-----------------------------------------------------------------
    std::valarray<T> Row( size_t row ) const {
        // slice (size_t start, size_t length, size_t stride)
        return elements[ std::slice( row * n_columns, n_columns, 1 ) ];
    }

    //-----------------------------------------------------------------
    // Write array to row
    //-----------------------------------------------------------------
    void WriteRow( size_t row, std::valarray<T> array ) {
        size_t N = array.size();
    
        if ( N != n_columns ) {
            std::stringstream errMsg;
            errMsg << "Matrix::insertRow(): row argument must be "
                   << n_columns << ". " << N << " were provided.\n";
            throw std::runtime_error( errMsg.str() );
        }
        for ( size_t i = 0; i < N; i++ ) {
            (*this)( row, i ) = array[ i ];
        }
    }

    //-----------------------------------------------------------------
    // Write array to col
    //-----------------------------------------------------------------
    void WriteColumn( size_t col, std::valarray<T> array ) {
        size_t N = array.size();
    
        if ( N != n_rows ) {
            std::stringstream errMsg;
            errMsg << "Matrix::insertColumn(): col argument must be "
                   << n_rows << ". " << N << " were provided.\n";
            throw std::runtime_error( errMsg.str() );
        }
        for ( size_t i = 0; i < N; i++ ) {
            (*this)( i, col ) = array[ i ];
        }
    }

    //-----------------------------------------------------------------
    // Return (sub)Matrix of specified column indices
    //-----------------------------------------------------------------
    Matrix<double> MatrixColumns( std::vector<size_t> columns ) {
        
        Matrix<double> M = Matrix( n_rows, columns.size() );

        size_t col_j = 0;
        
        for ( size_t i = 0; i < columns.size(); i++ ) {
            size_t col_i = columns[ i ];

            if ( col_i >= n_columns ) {
                std::stringstream errMsg;
                errMsg << "Matrix::MatrixColumns(): A column index ("
                       << col_i << ") exceeds the matrix domain.\n";
                throw std::runtime_error( errMsg.str() );
            }
            
            std::valarray<double> column_vec_i = Column( col_i );

            M.WriteColumn( col_j, column_vec_i );
            col_j++;
        }
        
        return M;
    }

    //-----------------------------------------------------------------
    // Build Column Name Index( std::string colNames )
    //-----------------------------------------------------------------
    void BuildColumnNameIndex( std::string colNames ) {
        // If colNames provided populate columnNames, columnNameToIndex
        if ( colNames.size() ) {
            columnNames = SplitString( colNames, " ,\t" );
            if ( columnNames.size() != n_columns ) {
                std::stringstream errMsg;
                errMsg << "Matrix::BuildColumnNameIndex(s) "
                       << "Number of column names ("
                       << columnNames.size() << ") does not match the"
                       << " number of columns (" << n_columns << ").\n";
                throw std::runtime_error( errMsg.str() );
            }
        }
        for ( size_t i = 0; i < columnNames.size(); i++ ) {
            columnNameToIndex[ columnNames[i] ] = i;
        }
    }

    //-----------------------------------------------------------------
    // Build Column Name Index
    //-----------------------------------------------------------------
    void BuildColumnNameIndex() {
        // If columnNames provided, populate columnNameToIndex
        if ( columnNames.size() ) {
            if ( columnNames.size() != n_columns ) {
                std::stringstream errMsg;
                errMsg << "Matrix::BuildColumnNameIndex() Number of column "
                       << " names (" << columnNames.size() 
                       << ") does not match the number of columns "
                       << "(" << n_columns << ").\n";
                throw std::runtime_error( errMsg.str() );
            }
        }
        for ( size_t i = 0; i < columnNames.size(); i++ ) {
            columnNameToIndex[ columnNames[i] ] = i;
        }
        
#ifdef DEBUG_ALL
        std::cout << "Matrix::BuildColumnNameIndex()\n";
        for ( size_t i = 0; i < columnNames.size(); i++ ) {
            std::cout << columnNames[i] << " : "
                      << columnNameToIndex[ columnNames[i] ] << "   ";
            columnNameToIndex[ columnNames[i] ] = i;
        } std::cout << std::endl;
#endif
    }
    
    //------------------------------------------------------------------
    // Print matrix to ostream
    //------------------------------------------------------------------
    friend std::ostream& operator <<( std::ostream& os, const Matrix& M ) {
        os << "Matrix: -----------------------------------\n";
        os << M.NRows() << " rows, " << M.NColumns() << " columns.\n";
        os << "---------------- First " << 10 << " rows ---------------\n";
        
        // print names of columns
        for ( size_t i = 0; i < M.ColumnNames().size(); i++ ) {
            os << M.ColumnNames()[i] << " \t";
        } os << std::endl;
        
        os << "----------------------------------------------\n";
 
        // print vec data up to maxRowPrint points
        for ( size_t row = 0; row < M.NRows() and row < 10; row++ ) {
            
            // print data points from each col
            for ( size_t col = 0; col < M.NColumns(); col++ ) {
                os << M( row, col ) << " \t";
            }
            os << std::endl;
        }
        os << "----------------------------------------------" << std::endl;

        return os;
    }
};
#endif

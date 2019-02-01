#ifndef DATAFRAME_H
#define DATAFRAME_H

// NOTE: This header deviates from the desired class implementation
// where *.h provides declarations, *.cc methods.  This is solely to
// accomodate the OSX XCode environment which seems unable to deal
// with c++11 standard template implemenations.
// A possible solution is to link against libc++ on OSX. See ../etc/.

#include "Common.h"

// Since #include DataFrame.h is in Common.h, need forward declaration
extern std::vector<std::string> SplitString( std::string inString, 
                                             std::string delimeters );

//---------------------------------------------------------
// DataFrame class
// Data container is a single, contiguous valarray.
// DataFrame access is through the () operator: (row,col).
//---------------------------------------------------------
template <class T>
class DataFrame {
    
    std::valarray<T> elements;
    size_t           n_columns;
    size_t           n_rows;
    
    std::vector< std::string >      columnNames;
    std::map< std::string, size_t > columnNameToIndex;
    
    size_t maxRowPrint;
    
public:
    //-----------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------
    DataFrame () {}
    
    //-----------------------------------------------------------------
    DataFrame( size_t rows, size_t columns ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ),
        maxRowPrint( 10 ) {}
    
    //-----------------------------------------------------------------
    DataFrame( size_t rows, size_t columns, std::string colNames ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ),
        columnNames( std::vector<std::string>(columns) ), maxRowPrint( 10 )
    {
        BuildColumnNameIndex( colNames );
    }
   
    //-----------------------------------------------------------------
    DataFrame( size_t rows, size_t columns,
            std::vector< std::string > columnNames ):
        n_rows( rows ), n_columns( columns ), elements( columns * rows ),
        columnNames( columnNames ), maxRowPrint( 10 )
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

    size_t &MaxRowPrint()       { return maxRowPrint; }
    size_t  MaxRowPrint() const { return maxRowPrint; }
    
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

    //------------------------------------------------------------------
    // Return data column selected by column name
    //------------------------------------------------------------------
    std::valarray< double > VectorColumnName( std::string column ) {
        
        std::vector< std::string >::iterator ci = std::find(columnNames.begin(),
                                                            columnNames.end(),
                                                            column );
        if ( ci == columnNames.end() ) {
            std::stringstream errMsg;
            errMsg << "DataFrame::VectorColumnName() Failed to find column: "
                   << column;
            errMsg << " in DataFrame columns:\n[ ";
            for ( auto cni  = columnNames.begin();
                       cni != columnNames.end(); ++cni ) {
                errMsg << *cni << " ";
            } errMsg << "]" << std::endl;
            throw std::runtime_error( errMsg.str() );
        }
        
        size_t col_i = std::distance( columnNames.begin(), ci );
        
        std::valarray<double> vec = Column( col_i );
        
        return vec;
    }

    //-----------------------------------------------------------------
    // Return (sub)DataFrame of specified column indices
    //-----------------------------------------------------------------
    DataFrame<double> DataFrameFromColumnIndex( std::vector<size_t> columns ) {
        
        DataFrame<double> M = DataFrame( n_rows, columns.size() );

        size_t col_j = 0;
        
        for ( size_t i = 0; i < columns.size(); i++ ) {
            size_t col_i = columns[ i ];

            if ( col_i >= n_columns ) {
                std::stringstream errMsg;
                errMsg << "DataFrame::DataFrameFromColumnIndex(): "
                       << "A column index ("
                       << col_i << ") exceeds the data frame domain.\n";
                throw std::runtime_error( errMsg.str() );
            }
            
            std::valarray<double> column_vec_i = Column( col_i );

            M.WriteColumn( col_j, column_vec_i );
            col_j++;
        }
        
        return M;
    }

    //------------------------------------------------------------------
    // Return (sub)DataFrame selected by columnNames
    // columnNames converted to column indices for DataFrameFromColumnIndex()
    //------------------------------------------------------------------
    DataFrame< double > DataFrameFromColumnNames(
        std::vector<std::string> colNames ) {

        // vector of column indices for dataDataFrame.DataFrameFromColumnIndex()
        std::vector<size_t> col_i_vec;
        
        // Map column names to indices
        std::vector<std::string>::iterator si;
        for ( auto ci = colNames.begin(); ci != colNames.end(); ++ci ) {
            auto si = find( columnNames.begin(), columnNames.end(), *ci );
            
            if ( si != columnNames.end() ) {
                col_i_vec.push_back( std::distance( columnNames.begin(), si ) );
            }
        }
        
        // Validation
        if ( col_i_vec.size() != colNames.size() ) {
            std::stringstream errMsg;
            errMsg << "DataFrame::DataFrameFromColumnNames() "
                      "Failed to find columns:\n[ ";
            for ( auto ci = colNames.begin(); ci != colNames.end(); ++ci ){
                errMsg << *ci << " ";
            } errMsg << "]" << std::endl;
            errMsg << "in DataFrame columns:\n[ ";
            for (auto ci = columnNames.begin(); ci != columnNames.end(); ++ci){
                errMsg << *ci << " ";
            } errMsg << "]" << std::endl;
            throw std::runtime_error( errMsg.str() );
        }
        
        DataFrame<double> M_col = DataFrameFromColumnIndex( col_i_vec );
        
        // Now insert the columnNames
        M_col.ColumnNames() = colNames;
        M_col.BuildColumnNameIndex();
        
        return M_col;
    }

    //-----------------------------------------------------------------
    // Write array to row
    //-----------------------------------------------------------------
    void WriteRow( size_t row, std::valarray<T> array ) {
        size_t N = array.size();
    
        if ( N != n_columns ) {
            std::stringstream errMsg;
            errMsg << "DataFrame::insertRow(): row argument must be "
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
            errMsg << "DataFrame::insertColumn(): col argument must be "
                   << n_rows << ". " << N << " were provided.\n";
            throw std::runtime_error( errMsg.str() );
        }
        for ( size_t i = 0; i < N; i++ ) {
            (*this)( i, col ) = array[ i ];
        }
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
                errMsg << "DataFrame::BuildColumnNameIndex(s) "
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
                errMsg << "DataFrame::BuildColumnNameIndex() Number of column "
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
        std::cout << "DataFrame::BuildColumnNameIndex()\n";
        for ( size_t i = 0; i < columnNames.size(); i++ ) {
            std::cout << columnNames[i] << " : "
                      << columnNameToIndex[ columnNames[i] ] << "   ";
            columnNameToIndex[ columnNames[i] ] = i;
        } std::cout << std::endl;
#endif
    }
    
    //------------------------------------------------------------------
    // Print DataFrame to ostream
    //------------------------------------------------------------------
    friend std::ostream& operator <<( std::ostream& os, const DataFrame& D ) {
        os << "DataFrame: -----------------------------------\n";
        os << D.NRows() << " rows, " << D.NColumns() << " columns.\n";
        os << "---------------- First " << D.MaxRowPrint()
           << " rows ---------------\n";
        
        // print names of columns
        for ( size_t i = 0; i < D.ColumnNames().size(); i++ ) {
            os << D.ColumnNames()[i] << " \t";
        } os << std::endl;
        
        os << "----------------------------------------------\n";
 
        // print vec data up to maxRowPrint points
        for ( size_t row = 0; row < D.NRows() and
                              row < D.MaxRowPrint(); row++ ) {
            
            // print data points from each col
            for ( size_t col = 0; col < D.NColumns(); col++ ) {
                os << D( row, col ) << " \t";
            }
            os << std::endl;
        }
        os << "----------------------------------------------" << std::endl;

        return os;
    }
};
#endif

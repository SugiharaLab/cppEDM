#ifndef DATAFRAME_H
#define DATAFRAME_H

// NOTE: This header deviates from the desired class implementation
// where *.h provides declarations, *.cc methods.  This is solely to
// accomodate the OSX XCode environment which seems unable to deal
// with c++11 standard template implemenations.
// A possible solution is to link against libc++ on OSX. See ../etc/.

#include <iomanip>
#include <fstream>
#include <iterator>

#include "Common.h"

// Since #include DataFrame.h is in Common.h, need forward declaration
extern std::vector<std::string> SplitString( std::string inString, 
                                             std::string delimeters = "," );
extern bool OnlyDigits( std::string str );

// Type definition for CSV NamedData to pair column names/column data
typedef std::vector<std::pair<std::string, std::vector<double>>> NamedData;

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
    DataFrame( std::string path, std::string fileName ):
         maxRowPrint( 10 )
    {
        //SetupDataFrame will take care of our member vars we need to setup
        NamedData csvInput = ReadData (path, fileName);
        SetupDataFrame (csvInput);
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
    std::valarray<T>  Elements() const { return elements; }
    std::valarray<T> &Elements()       { return elements; }
    
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
        // precision should be a parameter
        os.precision( 4 );
        os.fill( ' ' );
        os.setf( std::ios::fixed, std::ios::floatfield );
        
        os << "DataFrame: -----------------------------------\n";
        os << D.NRows() << " rows, " << D.NColumns() << " columns.\n";
        os << "---------------- First " << D.MaxRowPrint()
           << " rows ---------------\n";
        
        // print names of columns
        for ( size_t i = 0; i < D.ColumnNames().size(); i++ ) {
            os << std::setw(13) << D.ColumnNames()[i];
        } os << std::endl;
        
        os << "----------------------------------------------\n";
 
        // print vec data up to maxRowPrint points
        for ( size_t row = 0; row < D.NRows() and
                              row < D.MaxRowPrint(); row++ ) {
            
            // print data points from each col
            for ( size_t col = 0; col < D.NColumns(); col++ ) {
                os << std::setw(13) << D( row, col );
            }
            os << std::endl;
        }
        os << "----------------------------------------------" << std::endl;

        return os;
    }

    //------------------------------------------------------------------
    //  Write data to file
    //  @param outputFilePath: path to the file to write to
    //  @param outputFileName: filename to write to 
    //  @return: none
    //------------------------------------------------------------------
    void WriteData(std::string outputFilePath, std::string outputFileName) {
        //to hold the lines to print to the output file
        std::vector< std::string > fileLines;

        //tmp string to hold one line at a time
        std::stringstream lineStr;

        // Set stream precision. This should be a parameter.
        lineStr.precision( 4 );
        lineStr.setf( std::ios::fixed, std::ios::floatfield );

        //save col names line
        if ( ColumnNames().size() == 0 ) {
            std::cout << "DataFrame::WriteData(): This data frame has no column"
                      << "names.  Column names will be created.\n";
            for ( size_t i = 0; i < n_columns; i++ ) {
                std::stringstream colname;
                colname << "V" << i;
                ColumnNames().push_back( colname.str() );
            }
        }
        if ( ColumnNames().size() != n_columns ) {
            throw std::runtime_error( "DataFrame::WriteData(): The number of "
                                      " column names does not match the number "
                                      " of data columns.\n" );
        }
        for (size_t colIdx = 0; colIdx < n_columns; colIdx++) {
            lineStr << ColumnNames()[ colIdx ];

            if ( colIdx != n_columns - 1 ) {
                lineStr << ",";
            }
        }

        // set and empty
        fileLines.push_back( lineStr.str() );
        lineStr.str(std::string()); // would lineStr.flush() do the same?

        // iterate through all numerical data to print
        for (size_t rowIdx = 0; rowIdx < n_rows; rowIdx++) {
            for (size_t colIdx = 0; colIdx < n_columns; colIdx++) {

                lineStr << (*this) (rowIdx, colIdx);

                if ( colIdx != n_columns - 1 ) {
                    lineStr << ",";
                }
            }

            // set and empty
            fileLines.push_back( lineStr.str() );
            lineStr.str(std::string());
        }

        // write contents to file
        std::ofstream outputFile(outputFilePath + outputFileName);
        if (outputFile.is_open()) {

            std::copy(fileLines.begin(), fileLines.end(),
                    std::ostream_iterator<std::string>(outputFile,"\n"));

            outputFile.close();
        }

        // bad write if got to here
        else {
            std::stringstream errMsg;
            errMsg << "DataFrame::WriteData(): bad file permissions: "
                << outputFilePath + outputFileName << ". \n";
            throw std::runtime_error( errMsg.str() );
        }

    }

    //------------------------------------------------------------------
    // method to setup DataFrame
    // @param csvInput from ReadData()
    // @return: dataFrame
    //------------------------------------------------------------------
    void SetupDataFrame ( NamedData csvInput ) {

        // Setup column names in same order as dataFrame
        std::vector< std::string > colNames;
        for ( NamedData::iterator iterate = csvInput.begin(); 
              iterate != csvInput.end(); iterate++ ) {
            colNames.push_back( iterate->first );
        }
        
        // setup DataFrame members
        n_rows = csvInput.begin()->second.size();
        n_columns = csvInput.size();
        elements = std::valarray <T> (n_rows * n_columns);
        columnNames = colNames;
        BuildColumnNameIndex();

        // Transfer each data value into the class valarray 
        //    Another option is to use the writeColumn() method
        // csvInput is : pair< string, vector<double> >
        for ( NamedData::iterator iterate  = csvInput.begin(); 
                                  iterate != csvInput.end(); iterate++ ) {
            
            size_t colIdx = std::distance( csvInput.begin(), iterate );

            for ( size_t rowIdx = 0; rowIdx < n_rows; rowIdx++ ) {
                (*this)( rowIdx, colIdx ) = iterate->second[ rowIdx ];
            }
        }
    }
    
private:
    
    //----------------------------------------------------------------
    // 
    //----------------------------------------------------------------
    NamedData ReadData(std::string path, std::string fileName) {
        
        // Create input file stream and open file for input
        std::ifstream dataStrm( path + fileName );
        
        //make sure file access is good before reading
        if ( not dataStrm.is_open() ) {
            std::stringstream errMsg;
            errMsg << "ERROR: ReadData() file " << path + fileName
                   << " is not open for reading." << std::endl;
            throw std::runtime_error( errMsg.str() );
        }
        if ( not dataStrm.good() ) {
            std::stringstream errMsg;
            errMsg << "ERROR: ReadData() file " << path + fileName
                   << " is not ready for reading." << std::endl;
            throw std::runtime_error( errMsg.str() );
        }
        
        // Read fileName into a vector of strings, one line per string
        std::vector< std::string > dataLines;
        std::string tmp;
        
        while( getline( dataStrm, tmp ) ) {
            dataLines.push_back( tmp );
        }
        dataStrm.close();
        
#ifdef DEBUG_ALL
        std::cout << "------- ReadData() Contents of file "
                  << fileName << " -------" << std::endl;
        for( std::vector< std::string >::iterator ci = dataLines.begin();
             ci != dataLines.end(); ++ci ) {
            std::cout << *ci << std::endl;
        }
#endif
        
        // Container for data names/vectors
        NamedData namedData; // pair< string, vector< double >> NamedData;
        
        // Container of column names in the same order as in csv file
        std::vector< std::string > colNames;

        // Check first line to see if it's only numeric digits, or a header
        bool onlyDigits = true;
        std::vector<std::string> firstLineWords = SplitString( dataLines[0] );
        
        for ( auto si =  firstLineWords.begin();
                   si != firstLineWords.end(); ++si ){

            onlyDigits = OnlyDigits( *si );
            
            if ( not onlyDigits ) { break; }
        }
        if ( onlyDigits ) {
            // create named columns with generic col names: V1, V2...
            for (size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++){
                colNames.push_back( "V" + std::to_string(colIdx) );
            }
        }
        else {
            // get named columns from header line
            for (size_t colIdx = 0; colIdx < firstLineWords.size(); colIdx++){
                colNames.push_back( firstLineWords[colIdx] );  
            }
            // remove header line from read in lines so only numerical after
            dataLines.erase( dataLines.begin() );
        }
        
        // setup each col in namedData with new vec to insert numerical data to
        for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
            NamedData::value_type colPair ( colNames[colIdx],
                                            std::vector<double>());
            namedData.push_back ( colPair );
        }
        
        // Process each line in dataLines to fill in data vectors
        for ( size_t lineIdx = 0; lineIdx < dataLines.size(); lineIdx++ ) {
            std::vector<std::string> words = SplitString(dataLines[ lineIdx ]);
            for ( size_t colIdx = 0; colIdx < colNames.size(); colIdx++ ) {
                namedData[ colIdx ].second.push_back(std::stod( words[colIdx]));
            }
        }

#ifdef DEBUG_ALL
        std::cout << "------- ReadData() data from "
                  << fileName << " -------" << std::endl;
        for ( auto ci = namedData.begin(); ci != namedData.end(); ++ci ) {
            std::cout << ci->first << " ";
        } std::cout << std::endl;

        for (size_t row = 0; row < (namedData.begin()->second).size(); row++){
            for ( auto ci = namedData.begin(); ci != namedData.end(); ++ci ) {
                std::vector< double > vec = ci->second;
                std::cout << vec[ row ] << " ";
            } std::cout << std::endl;
        }
#endif
        
        return namedData;
    }
};
#endif

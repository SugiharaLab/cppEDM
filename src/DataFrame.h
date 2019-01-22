/* header file for the dataframe class. */

#include "Common.h"

/* this class is to provide a data storage container suited for
 * edm block and similar to the DataFrame of R/pandas py
 * */
class DataFrame {

    private:

        //underlying datastructure to the dataframe
        Matrix< double > container;

        std::vector< std::string > colNames;

    public:

        DataFrame (const std::string filepath, const std::string fileName); 
        
        Matrix< double > SetupContainer (const NamedData csvOutput);

        std::size_t NumColumns();
        std::size_t NumRows();
        std::vector< std::string > ColumnNames();
        friend std::ostream& operator<<(std::ostream& os, DataFrame& df);
        double & operator() (std::size_t rowIdx, std::size_t colIdx);
        double & operator() (std::size_t colIdx, std::string colName);
};

//probably should move this to matrix class
//to handle out of bounds / col name not found
struct BadColIdx : public std::exception {
   const char * what () const throw () {
      return "invalid column accessed";
   }
};
struct BadRowIdx : public std::exception {
   const char * what () const throw () {
      return "invalid row accessed";
   }
};

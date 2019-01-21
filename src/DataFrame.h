/* header file for the dataframe class. */

#include "Common.h"

/* this class is to provide a data storage container suited for
 * edm block and similar to the DataFrame of R/pandas py
 * */
class DataFrame {

    private:

        //underlying datastructure to the dataframe
        DFContainer container;

        ColNamesList colNames;

        int rowSize;

    public:

        DataFrame () {}
        DataFrame (const std::string filepath, int& validFile); 

        int NumColumns();
        int NumRows();
        ColNamesList ColumnNames();
        friend std::ostream& operator<<(std::ostream& os, DataFrame& df);
        double & operator() (int colIdx, int rowIdx);
        double & operator() (std::string colName, int rowIdx);
};

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

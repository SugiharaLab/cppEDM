/* file just contains a csv reader to read from csv 
 * todo: separate reading process into separate threads for faster reading 
 * */

#include "Common.h"
#include <fstream>

#define VALID_DOUBLE_CHARS "1234567890.,"

/* method to read in data from csv into underlying dataframe structure
 * note: the default name of the columns are (idx) starting at 0
 * @param filepath: the path to the csv file to check
 * @param container: vector of vector<double> to insert csv data to
 * @return: true if read was successful, false if not
 * */
bool ReadData (std::string filepath, NamedData & container) {
    //the separator for the parsing
    const std::string DELIM =  ",";
    //the file stream for the csv file
    std::ifstream file (filepath);    
    //to hold the text from each line in the csv and every word in line
    std::string currentLine;
    //to hold a container of split words and specifically the named columns
    std::vector < std::string > splitWords;
    std::vector < std::string > namesContainer;
    //to hold num representation
    double wordVal;
    //to hold the number of columns in the data
    int numCols;
    //to hold the data for more efficient inserts before dumping in container
    std::vector <std::vector < double > > numericContainer;

    // check validity of file 
    if ( !file.is_open() )
        return false;

    //get first line to check whether has column names or not
    std::getline(file, currentLine);
    //get each col name from header and add to column name container
    splitWords = SplitString (currentLine, DELIM);
    //if col names present process them, if not put back first line
    if (currentLine.find_first_not_of(VALID_DOUBLE_CHARS)!=std::string::npos) {
        namesContainer = splitWords;
        numCols += splitWords.size();
    }
    else {
        file.seekg(0, std::ios::beg);
        for (int idx = 0; idx < splitWords.size(); idx++)
            namesContainer.push_back(COL_PREFIX+std::to_string(idx));
    }
    currentLine.clear();

    //setup container with num vecs to hold just numeric csv data
    for (int colIdx = 0; colIdx < numCols; colIdx++)
        numericContainer.push_back(std::vector<double>());

    /* parse every line in csv file to extract data */
    while ( std::getline(file, currentLine) ) {
        //parse/tokenize line to get every word and add to vectors
        splitWords = SplitString (currentLine, DELIM);
        for (int colIdx = 0; colIdx < namesContainer.size(); colIdx++ ) {
            #ifdef READ_DATA_DEBUG
                std::cout<<splitWords[colIdx]<<std::endl;
                std::cout<<"size of vec is now "<<
                    numericContainer[colIdx].size()<<std::endl;
            #endif
            //could sanity check but expensive
            //push to appropriate vec and reset
                double parsedInt = std::stod(splitWords[colIdx]);
                numericContainer.at(colIdx).push_back(parsedInt);
        }
    }

    // setup csv data into map format 
    for (int colIdx = 0; colIdx < namesContainer.size(); colIdx++) {
        std::pair<std::string, std::vector<double> > col 
            (namesContainer[colIdx], numericContainer[colIdx]);
        container.insert(col);
    }

    //success
    file.close();
    return true;
}

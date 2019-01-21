/* file just contains a csv reader to read from csv 
 * todo: separate reading process into separate threads for faster reading 
 * */

#include "Common.h"
#include <fstream>

#define DELIM ","
#define VALID_DOUBLE_CHARS "1234567890.,"

/* method to read in data from csv into underlying dataframe structure
 * note: the default name of the columns are (idx) starting at 0
 * @param filepath: the path to the csv file to check
 * @param container: vector of vector<double> to insert csv data to
 * @return: true if read was successful, false if not
 * */
bool ReadData (std::string filepath, NamedData & container) {
    //the file stream for the csv file
    std::ifstream file (filepath);    
    //to hold the text from each line in the csv and every word in line
    std::string currentLine;
    char* currentWord;
    //to hold num representation
    double wordVal;
    //to hold the number of columns in the data
    int numCols;
    //to hold the data for more efficient inserts before dumping in container
    std::vector <std::vector < double > > numericContainer;
    //to hold the possible column names in the csv file header
    std::vector < std::string > namesContainer;

    // check validity of file 
    if ( !file.is_open() )
        return false;

    //get first line to check whether has column names or not
    std::getline(file, currentLine);
    //if col names process, if not put back first line
    if (currentLine.find_first_not_of(VALID_DOUBLE_CHARS)!=std::string::npos) {
        //get each col name from header and add to column name container
        currentWord = strtok(&currentLine[0], DELIM);
        while (currentWord) {
            namesContainer.push_back(currentWord);
            currentWord = strtok(NULL, DELIM);
            numCols++;
        }
    }
    else 
        file.seekg(0, std::ios::beg);
    currentLine.clear();

    //setup container with num vecs to hold just numeric csv data
    for (int colIdx = 0; colIdx < numCols; colIdx++)
        numericContainer.push_back(std::vector<double>());

    /* parse every line in csv file to extract data */
    while ( std::getline(file, currentLine) ) {
        //parse/tokenize line to get every word and add to vectors
        currentWord = strtok(&currentLine[0], DELIM);
        for (int colIdx = 0; currentWord && colIdx < numCols; colIdx++ ) {
            #ifdef READ_DATA_DEBUG
                std::cout<<std::stod(currentWord)<<std::endl;
                std::cout<<"size of vec is now "<<
                    numericContainer[colIdx].size()<<std::endl;
            #endif
            //could sanity check but expensive
            //push to appropriate vec and reset
            numericContainer[colIdx].push_back(std::stod(currentWord));
            currentWord = strtok(NULL, DELIM);
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

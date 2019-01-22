#include "DataFrame.cc"

int main () {

    //testing readData compatability
    NamedData container;
    DataFrame df ("../data/","SampleCSV.csv");;
    std::cout<<df<<std::endl;
    std::cout << df(0,0)<<std::endl<<std::flush;

}

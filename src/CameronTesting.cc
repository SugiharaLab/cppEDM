#include "DataFrame.cc"

int main () {

    //testing readData compatability
    NamedData container;
    int val = 0;
    DataFrame df ("../data/SampleCSV.csv",val);;
    std::cout<<df<<std::endl;
    std::cout << df("hi",0)<<std::endl<<std::flush;

}

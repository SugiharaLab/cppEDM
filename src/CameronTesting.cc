#include "DataFrame.h"
#include "Embed.cc"

int main () {

    //testing readData compatability
    NamedData container;
    DataFrame df ("../data/","SampleCSV.csv");;
    std::cout<<df<<std::endl;
    std::cout << df(0,0)<<std::endl<<std::flush;
    std::cout << df(0,"hi")<<std::endl<<std::flush;
    int E=2;
    int tau=1;
    std::vector<int> targets {0,1};
   //Matrix<double> embedReturn = EmbedData(E,tau,targets,df.DataMatrix());
    //std::cout<<embedReturn;
    //

    std::cout<<"done"<<std::endl<<std::flush;
    df.WriteData ("","testOutput.csv");
    std::cout<<"done2"<<std::endl<<std::flush;
}

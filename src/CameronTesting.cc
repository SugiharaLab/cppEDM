#include "Common.h"
#include "DataIO.h"
#include "Embed.cc"

int main () {

    //testing readData compatability
    NamedData container;
    DataIO dio ("../data/","SampleCSV.csv");
    std::cout << dio.DFrame() << std::endl;
    std::cout << dio.DFrame()(0,0) <<std::endl<<std::flush;
    int E=2;
    int tau=1;
    std::vector<int> targets {0,1};
   //DataFrame<double> embedReturn = EmbedData(E,tau,targets,dio.DataFrame());
    //std::cout<<embedReturn;
    //

    std::cout<<"done"<<std::endl<<std::flush;
    dio.WriteData ("","testOutput.csv");
    std::cout<<"done2"<<std::endl<<std::flush;

    //testing DataIO writing from dataframe instead of file
    DataFrame<double> df = dio.DFrame();
    df(5,5) = 400;
    std::cout<<df;
    DataIO nonFileData (df);
    nonFileData.WriteData("","testOutput2.csv");

    //testing new DataFrame with IO capabilities
    DataFrame<double> dfIOAbilities("../data/","SampleCSV.csv");
    std::cout<<"testing new DataFrame IO abilities\n";
    dfIOAbilities(5,1) = 420;
    dfIOAbilities(2,2) = 6969;
    std::cout<<dfIOAbilities;
    dfIOAbilities.WriteData("","testNewDataFrameIO.csv");
    

}

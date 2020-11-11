// Test simplex algorithm

#include "TestCommon.h"

int main () {

    // Declare DataFrames to hold file and computed output
    DataFrame< double > pyOutput;
    DataFrame< double > cppOutput;

    //----------------------------------------------------------
    // Simplex prediction on block_3sp data : No dynamic embedding
    //----------------------------------------------------------
    pyOutput = DataFrame < double >("./data/","Smplx_embd_block_3sp_pyEDM.csv");

    // Generate cpp output
    cppOutput = Simplex ( "../data/", "block_3sp.csv",
                          "./data/", "Smplx_embd_block_3sp_cppEDM.csv",
                          "1 99","100 198", 3, 1, 0, -1, 0,
                          "x_t y_t z_t", "x_t", true, false, false );
    // Comparison
    MakeTest ( "Simplex: block_3sp.csv embedded data",
               pyOutput, cppOutput );

    //----------------------------------------------------------
    // Simplex prediction with dynamically embedded data
    //----------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_E3_block_3sp_pyEDM.csv");

    // Generate cpp output
    cppOutput = Simplex ( "../data/", "block_3sp.csv",
                          "./data/", "Smplx_E3_block_3sp_cppEDM.csv",
                          "1 100", "101 195", 3, 1, 0, -1, 0,
                          "x_t", "x_t", false, false, false );
    // Comparison
    MakeTest ( "Simplex: block_3sp.csv dynamic embedding",
               pyOutput, cppOutput );

    //-------------------------------------------------------------------
    // Simplex prediction with dynamically embedded data and ISO Datetime
    //-------------------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_S12CD_E3_pyEDM.csv");

    // Generate cpp output
    cppOutput = Simplex ( "../data/", "S12CD-S333-SumFlow_1980-2005.csv",
                          "./data/", "Smplx_S12CD_E3_cppEDM.csv",
                          "1 800", "801 1375", 3, 1, 0, -1, 0,
                          "S12.C.D.S333", "S12.C.D.S333", false, false, false );

    cppOutput.MaxRowPrint() = 5; // Set number of rows to print

    // std::cout << cppOutput; // ISO datetime in Time column

    MakeTest ( "Simplex: S12CD-S333 ISO datetime", pyOutput, cppOutput );

    //----------------------------------------------------------
    // Simplex Ties
    //----------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_ties_valid.csv");

    // Generate cpp output
    cppOutput = Simplex ( "./data/", "tie.csv",
                          "./data/", "Smplx_ties_cppEDM.csv",
                          "1 10", "1 10", 1, 1, 0, -1, 0,
                          "ts", "ts", false, false, false );
    // Comparison
    MakeTest ( "Simplex: neighbor ties", pyOutput, cppOutput );

    //----------------------------------------------------------
    // Simplex Ties 2
    //----------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_ties2_valid.csv");

    // Generate cpp output
    cppOutput = Simplex ( "./data/", "tserr.csv",
                          "./data/", "Smplx_ties2_cppEDM.csv",
                          "1 100", "10 15", 1, 1, 0, -1, 0,
                          "ts", "ts", false, false, false );
    // Comparison
    MakeTest ( "Simplex: neighbor ties 2", pyOutput, cppOutput );

    //----------------------------------------------------------
    // Simplex disjoint lib
    //----------------------------------------------------------
    pyOutput = DataFrame < double > ("./data/","Smplx_disjointLib_valid.csv");

    // Generate cpp output
    cppOutput = Simplex ( "./data/", "small_data.csv",
                          "./data/", "Smplx_disjointLib_cppEDM.csv",
                          "1 3 6 12", "1 12", 0, 1, 0, -1, 0,
                          "x y", "x", true, false, false );
    // Comparison
    MakeTest ( "Simplex: disjoint library", pyOutput, cppOutput );
}

// Program to test the conditional embeddings functionality
// No ground truth to test on, so just checking for valid functionality here

#include <iostream>
#include <string>
#include "TestCommon.h" 

int main() {

    DataFrame < double > circleData( "../data/", "circle.csv" );

    size_t num_rows = 200;

    // The full and partial embeddings
    std::vector<bool> full_embedding (num_rows,true);
    std::vector<bool> partial_embedding_1 (num_rows,true);
    std::vector<bool> partial_embedding_2 (num_rows,true);
    for (int i=40;i<42;i++)  partial_embedding_1[i] = false;
    for (int i=40;i<42;i++) partial_embedding_2[i] = false;

    // The embedding assignment
    std::vector<std::vector<size_t>> emb_assignment (num_rows);
    for (int i=110;i<111;i++)  emb_assignment[i] = {1};
    for (int i=101;i<103;i++)  emb_assignment[i] = {2};
    for (int i=112;i<115;i++)  emb_assignment[i] = {1,2};

    std::vector<std::vector<bool>> embeddings = 
                    {full_embedding, partial_embedding_1,partial_embedding_2};

    SMapValues smapVals = SMap ( circleData,
                                 "./data/", "",
                                 " 1 100 ", "101 198", 2, 1, 0, -1, 4, 0,
                                 "x y", "x", "Smap_circle_coef.csv", "",
                                 emb_assignment,
                                 embeddings,
                                 true, false, false );
    
}

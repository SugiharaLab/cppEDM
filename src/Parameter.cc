
#include "Parameter.h"

//----------------------------------------------------------------
// Constructor
//----------------------------------------------------------------
Parameters::Parameters(
    std::string method,
    std::string lib,
    std::string pred,
    int         E,
    int         Tp,
    int         knn,
    int         tau,
    float       theta,
    float       svdSig,
    std::string jacobian,
    float       tikhonov,
    float       elasticNet,
    std::string colNames,
    std::string colIndex,
    std::string targNames,
    std::string targIndex,
    bool        embedded,
    int         multi,
    std::string libSizes,
    int         sample,
    bool        random,
    int         rseed,
    bool        noNeigh,
    bool        fwdTau,
    bool        verbose,
    std::string path,
    std::string dataFile,
    std::string predictFile,
    std::string SmapFile,
    std::string blockFile
    
    ) :
    // default variable initialization
    method ( ToLower(method).compare("simplex") ? Method::SMap :
                                                  Method::Simplex),
    E                ( E ),
    Tp               ( Tp ),
    knn              ( knn ),
    tau              ( tau ),
    theta            ( theta ),
    SVDSignificance  ( svdSig ),
    TikhonovAlpha    ( tikhonov ),
    ElasticNetAlpha  ( elasticNet ),
    columnNames      ( SplitString( colNames ) ),
    embedded         ( embedded ),
    MultiviewEnsemble( multi ),
    subSamples       ( sample ),
    randomLib        ( random ),
    seed             ( rseed ),
    noNeighborLimit  ( noNeigh ),
    forwardTau       ( fwdTau ),
    verbose          ( verbose )
{
    // Generate library indices and zero-offset
    std::vector<std::string> lib_str = SplitString( lib, " \t," );
    if ( lib_str.size() != 2 ) {
        std::string errMsg("Parameters(): library must be two integers.\n");
        throw std::runtime_error( errMsg );
    }
    int lib1 = std::stoi( lib_str[0] );
    int lib2 = std::stoi( lib_str[1] );

    library = std::vector<int>( lib2 - lib1 + 1 );
    std::iota ( library.begin(), library.end(), lib1 - 1 );

    // Generate prediction indices and zero-offset
    std::vector<std::string> pred_str = SplitString( pred, " \t," );
    if ( pred_str.size() != 2 ) {
        std::string errMsg("Parameters(): prediction must be two integers.\n");
        throw std::runtime_error( errMsg );
    }
    int pred1 = std::stoi( pred_str[0] );
    int pred2 = std::stoi( pred_str[1] );

    prediction = std::vector<int>( pred2 - pred1 + 1 );
    std::iota ( prediction.begin(), prediction.end(), pred1 - 1 );

#ifdef DEBUG
    std::cout << "Parameters(): library: ";
    for ( auto li = library.begin(); li != library.end(); ++li ) {
        std::cout << *li << " ";
    } std::cout << std::endl;
    std::cout << "Parameters(): prediction: ";
    for ( auto pi = prediction.begin(); pi != prediction.end(); ++pi ) {
        std::cout << *pi << " ";
    } std::cout << std::endl;
#endif

    // Jacobians
    std::vector<std::string> jac_str = SplitString( jacobian, " \t," );
    jacobians = std::vector<int>( jac_str.size() );
    for ( size_t i = 0; i < jac_str.size(); i++ ) {
        jacobians.push_back( std::stoi( jac_str[i] ) );
    }

    // columnIndex
    std::vector<std::string> col_i_str = SplitString( colIndex, " \t," );
    columnIndex = std::vector<int>( col_i_str.size() );
    for ( size_t i = 0; i < col_i_str.size(); i++ ) {
        columnIndex.push_back( std::stoi( col_i_str[i] ) );
    }


    
    // Validate and adjust parameters
    //Validate();
}

//----------------------------------------------------------------
// Destructor
//----------------------------------------------------------------
Parameters::~Parameters() {}

//----------------------------------------------------------------
// 
//----------------------------------------------------------------
void Parameters::Load() {}

//----------------------------------------------------------------
// Index offsets and validation
//----------------------------------------------------------------
void Parameters::Validate() {

    // If S-Map prediction, require k_NN > E + 1, default is all neighbors.
    // If Simplex and knn not specified, knn is set to E+1 in Simplex() ?JP
    if ( method == Method::SMap ) {
        if ( knn > 0 ) {
            if ( knn < E ) {
                std::stringstream errMsg;
                errMsg << "Parameters::Adjust() knn must be at least "
                          " E+1 (" << E+1 << ") with method S-Map.";
                throw std::runtime_error( errMsg.str() );
            }
        }
        if ( not embedded and columnNames.size() > 1 ) {
            std::string msg( "Parameters::Adjust() WARNING:  "
                             "Multivariable S-Map should use "
                             "-e (embedded) data input to ensure "
                             "data/dimension correspondance." );
            std::cout << msg;
        }

        // S-Map coefficient columns for jacobians start at 1 since the 0th
        // column is the S-Map linear prediction bias term
        if ( jacobians.size() > 1 ) {
            std::vector<int>::iterator it = find( jacobians.begin(),
                                                  jacobians.end(), 0 );
            if ( it != jacobians.end() ) {
                std::string errMsg( "Parameters::Adjust() S-Map coefficient "
                               " columns for jacobians can not use column 0.");
                throw std::runtime_error( errMsg );
            }
            if ( jacobians.size() % 2 ) {
                std::string errMsg( "Parameters::Adjust() S-Map coefficient "
                                    " columns for jacobians must be in pairs.");
                throw std::runtime_error( errMsg );                
            }
        }

        // Tikhonov and ElasticNet are mutually exclusive
        if ( TikhonovAlpha and ElasticNetAlpha ) {
            std::string errMsg( "Parameters::Adjust() Multiple S-Map solve "
                                "methods specified.  Use one or none of: "
                                "Tikhonov (-tr)  Elastic Net (-en).");
            throw std::runtime_error( errMsg );                
        }

        // Very small alphas don't make sense in elastic net
        if ( ElasticNetAlpha < 0.01 ) {
            std::cout << "Parameters::Adjust() ElasticNetAlpha too small."
                         " Setting to 0.01.";
            ElasticNetAlpha = 0.01;
        }
        if ( ElasticNetAlpha > 1 ) {
            std::cout << "Parameters::Adjust() ElasticNetAlpha too large."
                         " Setting to 1.";
            ElasticNetAlpha = 1;
        }
    }

}

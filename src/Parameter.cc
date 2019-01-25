
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
    std::string targetName,
    int         targetIndex,
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
    targetName       ( targetName ),
    targetIndex      ( targetIndex ),
    embedded         ( embedded ),
    MultiviewEnsemble( multi ),
    subSamples       ( sample ),
    randomLib        ( random ),
    seed             ( rseed ),
    noNeighborLimit  ( noNeigh ),
    forwardTau       ( fwdTau ),
    verbose          ( verbose ),
    path             ( path ),
    dataFile         ( dataFile ),
    predictOutputFile( predictFile ),
    SmapOutputFile   ( SmapFile ),
    blockOutputFile  ( blockFile ),
    validated        ( false )
{
    // Generate library indices: Not zero-offset
    std::vector<std::string> lib_vec = SplitString( lib, " \t," );
    if ( lib_vec.size() != 2 ) {
        std::string errMsg("Parameters(): library must be two integers.\n");
        throw std::runtime_error( errMsg );
    }
    int lib_start = std::stoi( lib_vec[0] );
    int lib_end   = std::stoi( lib_vec[1] );

    library = std::vector<int>( lib_end - lib_start + 1 );
    std::iota ( library.begin(), library.end(), lib_start );

    // Generate prediction indices: Not zero-offset
    std::vector<std::string> pred_vec = SplitString( pred, " \t," );
    if ( pred_vec.size() != 2 ) {
        std::string errMsg("Parameters(): prediction must be two integers.\n");
        throw std::runtime_error( errMsg );
    }
    int pred_start = std::stoi( pred_vec[0] );
    int pred_end   = std::stoi( pred_vec[1] );

    prediction = std::vector<int>( pred_end - pred_start + 1 );
    std::iota ( prediction.begin(), prediction.end(), pred_start );

#ifdef DEBUG_ALL
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
    if ( jacobian.size() > 0 ) {
        std::vector<std::string> jac_vec = SplitString( jacobian, " \t," );
        if ( jac_vec.size() < 2 ) {
            std::string errMsg( "Parameters(): jacobians must be at least "
                                "two integers.\n" );
            throw std::runtime_error( errMsg );
        }
        jacobians = std::vector<int>( jac_vec.size() );
        for ( size_t i = 0; i < jac_vec.size(); i++ ) {
            jacobians.push_back( std::stoi( jac_vec[i] ) );
        }
    }

    // columnIndex
    if ( colIndex.size() > 0 ) {
        std::vector<std::string> col_i_vec = SplitString( colIndex, " \t," );
        columnIndex = std::vector<int>( col_i_vec.size() );
        for ( size_t i = 0; i < col_i_vec.size(); i++ ) {
            columnIndex.push_back( std::stoi( col_i_vec[i] ) );
        }
    }

    // librarySizes
    if ( libSizes.size() > 0 ) {
        std::vector<std::string> libsize_vec = SplitString( libSizes, " \t," );
        if ( libsize_vec.size() != 3 ) {
            std::string errMsg( "Parameters(): librarySizes must be three "
                                "integers.\n" );
            throw std::runtime_error( errMsg );
        }
        for ( size_t i = 0; i < libsize_vec.size(); i++ ) {
            librarySizes.push_back( std::stoi( libsize_vec[i] ) );
        }
    }
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

    validated = true;

    // Apply zero-offsets
    // Convert library and prediction indices to zero-offset
    if ( library[0] < 1 ) {
        std::string errMsg( "Parameters::Validate() library start index must "
                            "be at least 1.");
        throw std::runtime_error( errMsg );                
    }
    if ( prediction[0] < 1 ) {
        std::string errMsg( "Parameters::Validate() prediction start index"
                            " must be at least 1.");
        throw std::runtime_error( errMsg );                
    }

    for ( auto li = library.begin(); li != library.end(); ++li ) {
        *li = *li - 1;
    }
    for ( auto pi = prediction.begin(); pi != prediction.end(); ++pi ) {
        *pi = *pi - 1;
    }    

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

//------------------------------------------------------------------
// Print to ostream
//  @param os: the stream to print to
//  @return: the stream passed in
//------------------------------------------------------------------
std::ostream& operator<< (std::ostream &os, Parameters &p ) {

    // print info about the dataframe
    os << "Parameters: -------------------------------------------\n";

    os << "Method: " << ( p.method == Method::Simplex ? "Simplex" : "SMap" )
       << " E=" << p.E << " Tp=" << p.Tp
       << " knn=" << p.knn << " tau=" << p.tau << " theta=" << p.theta
       << std::endl;

    os << "Library: [" << p.library[0] << " : "
       << p.library[ p.library.size() - 1 ] << "]  "
       << "Prediction: [" << p.prediction[0] << " : "
       << p.prediction[ p.prediction.size() - 1 ]
       << "] " << std::endl;

    
    os << "-------------------------------------------------------\n";
    
    return os;
}


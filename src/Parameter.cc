
#include "Parameter.h"

//----------------------------------------------------------------
// Constructor
//----------------------------------------------------------------
Parameters::Parameters() :
    // default variable initialization
    method( Method::Simplex ),
    library   ( std::vector<int>(2,0) ),
    prediction( std::vector<int>(2,0) ),
    E( 0 ), Tp( 1 ), knn( 0 ), tau( 1 ), theta( 0. ),
    SVDSignificance( 1E-5 ), TikhonovAlpha( 0 ), ElasticNetAlpha( 0 ),
    columnIndex( 1 ), targetIndex( 0 ), embedded( false ),
    MultiviewEnsemble( 0 ), librarySizes( std::vector<int>(5,0) ),
    subSamples( 100 ), randomLib( true ), seed( 0 ),
    noNeighborLimit( false ), forwardTau( false ), verbose( false ),
    path( "./" )
{}

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
void Parameters::Adjust() {

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

    // Convert library and prediction indices to zero-offset
    if ( library[0] < 1 ) {
        std::string errMsg( "Parameters::Adjust() library start index must "
                            "be at least 1.");
        throw std::runtime_error( errMsg );                
    }
    if ( prediction[0] < 1 ) {
        std::string errMsg( "Parameters::Adjust() prediction start index must "
                            "be at least 1.");
        throw std::runtime_error( errMsg );                
    }

    for ( auto li = library.begin(); li != library.end(); ++li ) {
        *li = *li - 1;
    }
    for ( auto pi = prediction.begin(); pi != prediction.end(); ++pi ) {
        *pi = *pi - 1;
    }
}

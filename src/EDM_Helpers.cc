// This file is for the helper functions (parsing ranges, validating ranges,...)
// that are only used in EDM which prevents us adding it to Common, but is
// extraneous to the actual EDM behavior
// Doesn't warrant a header file since only used in EDM.cc

#include "Common.h"

//----------------------------------------------------------------
// Distance computation between two vectors for several metrics
//----------------------------------------------------------------
double Distance( const std::valarray<double> &v1,
                 const std::valarray<double> &v2,
                 DistanceMetric metric )
{
    double distance = 0;

    // For efficiency sake, we forego the usual validation of v1 & v2.

    if ( metric == DistanceMetric::Euclidean ) {
        double sum   = 0;
        double delta = 0;
        for ( size_t i = 0; i < v1.size(); i++ ) {
            delta = v2[i] - v1[i];
            sum  += delta * delta; // avoid call to pow()
        }
        distance = sqrt( sum );

        // Note: this implicit implementation is slower
        // std::valarray<double> delta = v2 - v1;
        // distance = sqrt( (delta * delta).sum() );
    }
    else if ( metric == DistanceMetric::Manhattan ) {
        double sum = 0;
        for ( size_t i = 0; i < v1.size(); i++ ) {
            sum += abs( v2[i] - v1[i] );
        }
        distance = sum;
    }
    else {
        std::stringstream errMsg;
        errMsg << "Distance() Invalid DistanceMetric: "
               << static_cast<size_t>( metric );
        throw std::runtime_error( errMsg.str() );
    }

    return distance;
}

//----------------------------------------------------------
// Method to validate dataFrameIn rows against lib and pred indices
// prediction_max_i and library_max_i are the last time index for each range
// numRows    : number of rows in the input dataframe
// maxPredIdx : the max index in the pred indices
// maxLibIdx  : the max index in the lib indices
// return     : none, throws runtime error if invalid ranges        
//----------------------------------------------------------
void CheckDataRows( size_t numRows, size_t maxPredIdx, size_t maxLibIdx,
                    int E, int tau, bool embedded ){

    // param.prediction & library have been zero-offset in Validate()
    // to convert from user specified data row to array indicies

    size_t shift = embedded ? 0 : abs( tau ) * ( E - 1 );

    if ( numRows <= maxPredIdx ) {
        std::stringstream errMsg;
        errMsg << "CheckDataRows(): The prediction index " << maxPredIdx + 1
               << " exceeds the number of data rows " << numRows;
        throw std::runtime_error( errMsg.str() );
    }
    
    if ( numRows <= maxLibIdx + shift ) {
        std::stringstream errMsg;
        errMsg << "CheckDataRows(): The library index " << maxLibIdx + 1
               <<  " + tau(E-1) " << shift << " = " << maxLibIdx + 1 + shift
               << " exceeds the number of data rows " << numRows;
        throw std::runtime_error( errMsg.str() );
    }

}

//----------------------------------------------------------------
// Function to parse a potentially disjoint range string into its indices
// rangeStr  : range string to parse
// return    : vector of corresponding indices for given range string
//----------------------------------------------------------------
std::vector<size_t> ParseRangeStr ( std::string rangeStr ) {
    
    // Validate that number of ranges is even  
    std::vector<std::string> rangeVec = SplitString( rangeStr, " \t," );
    if ( rangeVec.size() % 2 != 0 ) {
        std::string errMsg( "Parameters::Validate(): "
                    "disjoint range must be even number of integers.\n" );
        throw std::runtime_error( errMsg );
    }

    // Generate vector of start, stop index pairs
    std::vector< std::pair< size_t, size_t > > rangePairs;
    for ( size_t i = 0; i < rangeVec.size(); i = i + 2 ) {
        rangePairs.emplace_back( std::make_pair(std::stoi( rangeVec[i]),
                                                std::stoi( rangeVec[i+1])));
    }

    // Create library vector of indices

    std::vector<size_t> rangeIndicesVec;

    for ( auto thisPair : rangePairs ) {
        for ( size_t li = thisPair.first; li <= thisPair.second; li++ ) {

            rangeIndicesVec.push_back( li - 1 ); // apply zero-offset

        }
    }

    // Assert size of range
    if ( not rangeIndicesVec.size() ) {
        std::string errMsg( "Parameters::Validate(): "
                            "prediction or library range not found.\n" );
        throw std::runtime_error( errMsg );
    }

    return rangeIndicesVec;

}

//----------------------------------------------------------------
// Function to parse multi-arg str into int vector (column indices)
// If column string contains only names, not indices, empty vector returned
//----------------------------------------------------------------
std::vector<size_t> ParseColumnIndices ( std::string indicesStr ) {

    // If columns are purely integer, then populate vector<size_t> columnIndex
    // Else return empty vector as no indices specified
    if ( indicesStr.size() ) {
        
        std::vector<std::string> columnSplit = SplitString( indicesStr,
                                                            " \t,\n" );
        bool onlyDigits = false;
        
        for ( auto ci = columnSplit.begin(); ci != columnSplit.end(); ++ci ) {
            onlyDigits = OnlyDigits( *ci, true );
            
            if ( not onlyDigits ) { break; }
        }
        
        if ( onlyDigits ) {
            std::vector<size_t> columnIndices;
            for ( auto columnIndex : columnSplit ) {
                columnIndices.push_back( std::stoi( columnIndex ) );
            }
            return columnIndices;
        }
    }

    // Either column str empty or contained alpha characters
    return std::vector<size_t>();
}
//----------------------------------------------------------------
// Function to parse multi-arg str into str vector (column names)
// If column string contains only indices, not names, empty vector returned
//----------------------------------------------------------------
std::vector<std::string> ParseColumnNames ( std::string columnsStr ) {

    // Fill columns into vector<string> columnNames
    if ( columnsStr.size() ) {
        
        std::vector<std::string> columnsVec = SplitString( columnsStr, 
                                                            " \t,\n" );
        
        bool onlyDigits = false;
        
        for ( auto ci = columnsVec.begin(); ci != columnsVec.end(); ++ci ) {
            onlyDigits = OnlyDigits( *ci, true );
            
            if ( not onlyDigits ) { break; }
        }
        
        if ( not onlyDigits ) {
            return columnsVec;
        }
    }

    // Either column str empty or contained only indices 
    return std::vector<std::string>();
}
//------------------------------------------------------------
// Adjust lib/pred concordant with Embed() removal of tau(E-1)
// rows, and DeletePartialDataRow()
// shift    : the shift amount (number of deleted rows)
// tau      : tau
// rangeVec : the range vector (lib or pred)
// return   : the adjusted range vectors
//------------------------------------------------------------
std::vector<size_t> AdjustRange(size_t shift, int tau, 
                                std::vector<size_t> rangeVec){

    // If [0, 1, ... shift]  (negative tau) or
    // [N-shift, ... N-1, N] (positive tau) are in library or prediction
    // those rows were deleted, delete these index elements.
    // First, create vectors of indices to delete.
    std::vector< size_t > deleted_range_elements( shift, 0 );

    if ( tau < 0 ) {
        std::iota(deleted_range_elements.begin(),deleted_range_elements.end(),0);
    }
    else {
        std::iota( deleted_range_elements.begin(), deleted_range_elements.end(),
                    rangeVec.size() - shift);
    }

    // Erase elements of row indices that were deleted
    for ( auto element  = deleted_range_elements.begin();
               element != deleted_range_elements.end(); element++ ) {

        std::vector< size_t >::iterator it;
        it = std::find( rangeVec.begin(), rangeVec.end(), *element );

        if ( it != rangeVec.end() ) {
            rangeVec.erase( it );
        }
    }
            
    // Now offset all values by shift so that vectors indices
    // in library and prediction refer to the same data rows
    // before the deletion/shift.
    if ( tau < 0 ) {
        for ( auto ri = rangeVec.begin(); ri != rangeVec.end(); ri++ ) {
            *ri = *ri - shift;
        }
    }
    // tau > 0  : Forward shifting: no adjustment needed from origin

    return rangeVec;
}




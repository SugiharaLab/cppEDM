// Test parsing and incrementing of datetime std::strings

#include <iostream>
#include <string>
#include "TestCommon.h" 

//-----------------------------------------------------
// FORMATS TESTED:
//   YYYY-MM-DD
//   HH:MM:SS
//   YYYY-MM-DD HH:MM:SS  (2019-06-30 10:26:10)
//   YYYY-MM-DDTHH:MM:SS  (2019-06-30T10:26:10)
//
//   #ifdef SUBSECOND_SUPPORTED hh:mm:ss.sss
//-----------------------------------------------------

//-----------------------------------------------------
// Compare IncrementDatetime() output to correct value
//-----------------------------------------------------
void Validate( std::string format,
               std::string date_str_1,
               std::string date_str_2, 
               std::string correct_incremented,
               int         tp ) {

    std::cout << '\t' << format << ": " << std::flush;

    std::string output = IncrementDatetime( date_str_1, date_str_2, tp );

    if ( output == correct_incremented )
        std::cout << GREEN_TEXT << "PASSED." << RESET_TEXT << std::endl;
    else 
        std::cout << RED_TEXT << "FAIL. " << RESET_TEXT << std::endl
                  << "Datetimes:" << date_str_1 << " and " << date_str_2
                  << ", tp increment: " << tp << std::endl
                  << "output: " << output
                  << " expected: " << correct_incremented << std::endl;
}

//-----------------------------------------------------
int main() {

    std::string date_str_1;
    std::string date_str_2;

    std::cout << "-------------------------------------------------\n"
              << "Test: DateTime\n"
              << "-------------------------------------------------"
              << std::endl;

    //---------------------------------------------
    // YYYY-MM-DD
    //---------------------------------------------
    // Note different number of days in months
    // 31 day difference
    date_str_1 = "2019-01-15";
    date_str_2 = "2019-02-15";
    Validate( "YYYY-MM-DD", date_str_1, date_str_2,
              "2019-03-18", 1 );
    // 7 day differnce
    date_str_1 = "2019-11-18";
    date_str_2 = "2019-11-25";
    Validate( "YYYY-MM-DD", date_str_1, date_str_2,
              "2019-12-02", 1 );
    // 3 day difference, tp of 2
    date_str_1 = "2019-01-24";
    date_str_2 = "2019-01-27";
    Validate(  "YYYY-MM-DD", date_str_1, date_str_2,
               "2019-02-02", 2 );

    //---------------------------------------------
    // HH:MM:SS
    //---------------------------------------------
    date_str_1 = "05:30:20";
    date_str_2 = "05:31:20";
    Validate( "HH:MM:SS", date_str_1, date_str_2,
                             "05:32:20", 1 );
    
    date_str_1 = "12:59:58";
    date_str_2 = "12:59:59";
    Validate( "HH:MM:SS", date_str_1, date_str_2,
              "13:00:01", 2 );

    //---------------------------------------------
    // YYYY-MM-DD HH:MM:SS
    //---------------------------------------------
    date_str_1 = "2019-06-30 23:59:58";
    date_str_2 = "2019-06-30 23:59:59";
    Validate( "YYYY-MM-DD HH:MM:SS", date_str_1, date_str_2,
              "2019-07-01 00:00:00", 1 );

    //---------------------------------------------
    // YYYY-MM-DDTHH:MM:SS
    //---------------------------------------------
    date_str_1 = "2019-06-30T23:59:58";
    date_str_2 = "2019-06-30T23:59:59";
    Validate( "YYYY-MM-DDTHH:MM:SS", date_str_1, date_str_2,
              "2019-07-01T00:00:00", 1 );

#ifdef SUBSECOND_SUPPORTED
    //---------------------------------------------
    // hh:mm:ss.sss
    //---------------------------------------------
    date_str_1 = "05:30:20.444";
    date_str_2 = "05:30:20.555";
    Validate( "hh:mm:ss.sss", date_str_1, date_str_2,
              "05:30:21", 1 );
#endif

    return 0;
}

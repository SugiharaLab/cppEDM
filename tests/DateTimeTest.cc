//file to test the parsing and incrementing of some datetime std::strings

#include <iostream>
#include <string>
#include "TestCommon.h" 

/*
   TIME FORMATS NEED TO TEST ON:
   YYYY-MM-DD
   HH:MM:SS
   YYYY-MM-DDTHH:MM:SS   (2019-06-30T10:26:10)
   hh:mm:ss.sss
*/

void check_increment_correct( std::string date_str_1, std::string date_str_2, 
        std::string correct_incremented, int tp ) {
    std::string output = increment_datetime_str( date_str_1, date_str_2, tp ); 
    if ( output == correct_incremented ) 
        std::cout <<"Correct."<<std::endl;
    else 
        std::cout << "BAD:"<<std::endl
        <<"datetimes are :"<<date_str_1<<" and "<<date_str_2
        <<", increment of tp "<<tp<<" is "<<std::endl
        <<output <<"vs"<<correct_incremented<<"end"<<std::endl
        <<"_______________"<<std::endl;
}

int main() {

    std::string date_str_1;
    std::string date_str_2;

    //////////////////testing YYYY-MM-DD//////////////////
    //difference in a month's unit. note won't be clean in months w date on 
    //same day cus different num days in months
    date_str_1 = "2019-01-30";
    date_str_2 = "2019-02-30";
    check_increment_correct( date_str_1, date_str_2, "2019-04-02",1 );
    //differnce in 7 days
    date_str_1 = "2019-11-18";
    date_str_2 = "2019-11-25";
    check_increment_correct( date_str_1, date_str_2, "2019-12-02",1 );
    //differnce in days - mult of 3 tp of 2
    date_str_1 = "2019-01-24";
    date_str_2 = "2019-01-27";
    check_increment_correct( date_str_1, date_str_2, "2019-02-02",2 );

    //////////////////testing HH:MM:SS//////////////////
    date_str_1 = "05:30:20";
    date_str_2 = "05:31:20";
    check_increment_correct( date_str_1, date_str_2, "05:32:20",1 );
    date_str_1 = "12:59:58";
    date_str_2 = "12:59:59";
    check_increment_correct( date_str_1, date_str_2, "13:00:01",2 );

    //////////////////testing YYYY-MM-DDTHH:MM:SS///////
    date_str_1 = "2019-06-30T23:59:58";
    date_str_2 = "2019-06-30T23:59:59";
    check_increment_correct( date_str_1, date_str_2, "2019-07-01T00:00:00",1 );

    //////////////////testing YYYY-MM-DDTHH:MM:SS///////
    date_str_1 = "05:30:20.444";
    date_str_2 = "05:30:20.555";
    check_increment_correct( date_str_1, date_str_2, "05:30:21",1 );

    return 0;
}

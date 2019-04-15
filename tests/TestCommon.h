#ifndef TESTCOMMON_H
#define TESTCOMMON_H

// #define PRINT_DIFFERENCE_IN_RESULTS

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include "Common.h"

#define STR_LINE_SEP "-------------------------------------------------"
#define TAB_CHAR     '\t'            

const float EPSILON = .2;

//consts for different output color
const std::string RED_TEXT   ("\033[0;31m");
const std::string GREEN_TEXT ("\033[1;32m");
const std::string YELLOW_TEXT("\033[1;33m");
const std::string RESET_TEXT ("\033[0m"   );

void MakeTest ( std::string testName,
                DataFrame< double > data1, 
                DataFrame< double > data2 );
#endif

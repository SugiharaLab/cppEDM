
#ifndef EDM_CCM_H
#define EDM_CCM_H

#include <cstdlib>
#include <random>
#include <unordered_set>
#include <chrono>
#include <queue>
#include <thread>

#include "EDM.h"
#include "Simplex.h"

//----------------------------------------------------------------
// CCM class inherits from Simplex class and defines
// CCM-specific projection methods
//----------------------------------------------------------------
class CCMClass : public SimplexClass {
public:
    SimplexClass colToTargetCCM; // object for column to target mapping
    SimplexClass targetToColCCM; // object for target to column mapping

    // Constructor
    CCMClass ( DataFrame< double > & data,
               Parameters          & parameters );

    // Method declarations
    void Project();
    void SetupParameters();
    void CopyData();
    void CCM();
    void FormatOutput();
    void WriteOutput();
};
#endif

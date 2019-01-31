#ifndef EMBED_H
#define EMBED_H

#include "Common.h"
#include "Parameter.h"
#include "DataFrame.h"

// Overloaded Embed functions : Type 1 with path & dataFile
Matrix< double > Embed ( std::string path     = "",
                         std::string dataFile = "",
                         int         E        = 0,
                         int         tau      = 0,
                         std::string columns  = "",
                         bool        verbose  = false );

// Overloaded Embed functions : Type 2 with dataMatrix
Matrix< double > Embed ( Matrix< double > dataMatrix,
                         int              E          = 0,
                         int              tau        = 0,
                         std::string      columns    = "",
                         bool             verbose    = false );

Matrix< double > MakeBlock ( Matrix< double >         dataMatrix,
                             int                      E,
                             int                      tau,
                             std::vector<std::string> columnNames,
                             bool                     verbose );
#endif

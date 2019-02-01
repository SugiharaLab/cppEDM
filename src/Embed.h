#ifndef EMBED_H
#define EMBED_H

#include "Common.h"
#include "Parameter.h"
#include "DataIO.h"

// Overloaded Embed functions : Type 1 with path & dataFile
DataFrame< double > Embed ( std::string path     = "",
                            std::string dataFile = "",
                            int         E        = 0,
                            int         tau      = 0,
                            std::string columns  = "",
                            bool        verbose  = false );

// Overloaded Embed functions : Type 2 with dataFrame
DataFrame< double > Embed ( DataFrame< double > dataFrame,
                            int                 E       = 0,
                            int                 tau     = 0,
                            std::string         columns = "",
                            bool                verbose = false );

DataFrame< double > MakeBlock ( DataFrame< double >      dataFrame,
                                int                      E,
                                int                      tau,
                                std::vector<std::string> columnNames,
                                bool                     verbose );
#endif

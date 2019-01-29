#ifndef EMBED_H
#define EMBED_H

#include "Common.h"
#include "Parameter.h"
#include "DataFrame.h"

Matrix< double > Embed ( std::string path            = "",
                         std::string dataFile        = "",
                         int         E               = 0,
                         int         tau             = 0,
                         std::string columns         = "",
                         bool        verbose         = false );

#endif

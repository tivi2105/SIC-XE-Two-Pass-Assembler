#ifndef PASSI_H
#define PASSI_H

#include "Validations.h"
#include "Parser.h"
#include "EvaluationHelper.h"
#include "Operations.h"
#include "OpTab.h"
#include "StringHelper.h"
#include "IOHandler.h"
#include "Structs.h"
#include <stdexcept>
#include <algorithm>
#include <math.h>

metaData runPass1(vector<transformLine> v, bool* started, bool* ended,
        bool* successfull);

#endif // PASSI_H

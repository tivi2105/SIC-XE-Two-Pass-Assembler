#ifndef OPCODE_H
#define OPCODE_H

#include <iostream>
#include <stdexcept>
#include "Parser.h"
#include "OpTab.h"
#include "StringHelper.h"
#include "Structs.h"
#include "Operations.h"
#include "EvaluationHelper.h"

using namespace std;

opCode getOpCode(transformLine pl, int loc_ctr, metaData* current);

string evaluateOpcode(opCode op);

void castBinaryToDecimal(bitset<6> flags, int* ni_val, int* xbpe_val);

#endif // OPCODE_H

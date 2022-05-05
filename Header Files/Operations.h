#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <math.h>
#include "EvaluationHelper.h"
#include "StringHelper.h"
#include <stdexcept>
#include "Structs.h"

using namespace std;
int valueOfLiteralOperand(string op);
int evaluateOperand(string operand, metaData* metadata = NULL);
bool isOp(string operand) ;
transformLine parse(string line);
int findExpressionType(string op);
int addressOfLiteralOperand(string op);

#endif // OPERATIONS_H

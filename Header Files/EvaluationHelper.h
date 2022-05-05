#ifndef EVALUATIONHELPER_H
#define EVALUATIONHELPER_H

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <iostream>
#include "Structs.h"
#include "StringHelper.h"
#include <stdexcept>

using namespace std;

bool validateExpression(string exp);

int evaluateExpression(string exp, metaData* cs, bool placeHolderVals= false);

#endif // EVALUATIONHELPER_H

#ifndef OPTAB_H
#define OPTAB_H

#include <map>
#include <string>

#include "Structs.h"

using namespace std;

const unsigned char FORMAT_1 = 1;
const unsigned char FORMAT_2 = 2;
const unsigned char FORMAT_3_4 = 3;

void deleteOpTab();
map<string, OpInfo*>* getOpTab();

#endif // OPTAB_H

#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include "Parser.h"
#include "OpTab.h"
#include <string>
#include "StringHelper.h"
#include "Operations.h"
#include "EvaluationHelper.h"
using namespace std;

int validate(transformLine pl);
bool isRubbish(string x);
bool isChar(char x);
bool isNumber (char a);
bool isReg(string o);
int f1(string o1, string o2);
int f2(string op,string o1, string o2);
int f34(string op,string o1, string o2);
void fillDirectives();
bool checkBYTE(string x);
bool checkLiteral(string x, bool isReg);
bool checkExt(string s);

#endif // VALIDATIONS_H

#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <string>
#include <iostream>
#include <vector>
#include "sstream"

using namespace std;

string intToString(int integer, bool hex);

void setLineLen(string* line, int length, char filling, bool lhs);

string toUpperCase(string x);

string trim(string x);

bool isNumeric(string s);

bool isHexa(string s);

int parseDecimal(string x);

int parseHexaDecimal(string x);

float parseFloat(string x);

vector<string> split(string x, char s);

string mergeStrings(string x, string y, char del);

vector<string> generalSplit(string toSplit, string splitters);

#endif // STRINGHELPER_H

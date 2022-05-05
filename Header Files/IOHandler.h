#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <map>
#include "StringHelper.h"
#include "Structs.h"
#include "OpCode.h"
#include "ui_mainwindow.h"

using namespace std;

#define FILE_LINE_WIDTH 63;
#define PASS_2_WIDTH 89;
#define LINE_NUMBER_WIDTH 16;
#define ADDRESS_WIDTH 15;
#define LABEL_WIDTH 9;
#define MNEMONIC_WIDTH 8;
#define OPERANDS_WIDTH 16;
#define FLAGS_WIDTH 15;
#define OPCODE_WIDTH 10;
#define SPACE_CHAR 0x20;
#define ZERO_CHAR 0x30
#define ASTERISK_CHAR 0x7E;
#define DASH_CHAR 0x5F;
#define NEW_LINE_CHAR 0x0A;
#define EQUAL_CHAR 0x5F;
#define TAB_CHAR 0x09;

string createLine(int lineNumber, string label, string op, string operands);

string createLineForPass2(int address, string label, string op_str,
        string operands, bitset<6> flags, string opCode);

void deleteFile(string name);

void createLine(ofstream* file, string line);

string readLine(ifstream* file);

void createTitle(ofstream* file);

void createTitleForPass2(ofstream* file);

void createError(ofstream* file, string error);

void createBorder(ofstream* file);

void createBorder_Pass2(ofstream* file);

void createSymTab(Ui::MainWindow *ui, ofstream* file, map<string, symInfo*>* theMap);

void createComment(ofstream* file, string comment);

void createTitleRecord(ofstream* file, string prog_name, int starting_add, int prog_length);

void createTextRecords(ofstream* file, vector<string>* opCodes, vector<locatedParsedLine>* lpl);

void createModRecords(ofstream* file, vector<modification>* mods);

void createDefineRecord(ofstream* file, vector<string>* exdefs, map<string, symInfo*>* theMap);

void createReferRecord(ofstream* file, vector<string>* exrefs);

void createEndRecord();

#endif // IOHANDLER_H

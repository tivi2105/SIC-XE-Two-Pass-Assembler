#include "OpTab.h"

map<string, OpInfo*>* opTable = NULL;

static void setOpTab() {

    OpInfo* opTabMap;

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "FIX";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xC4;
    opTable->insert(pair<string, OpInfo*>("FIX", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "NORM";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xC8;
    opTable->insert(pair<string, OpInfo*>("NORM", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "TIO";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xF8;
    opTable->insert(pair<string, OpInfo*>("TIO", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SIO";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xF0;
    opTable->insert(pair<string, OpInfo*>("SIO", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "FLOAT";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xC0;
    opTable->insert(pair<string, OpInfo*>("FLOAT", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "HIO";
    opTabMap->format = FORMAT_1;
    opTabMap->opCode = 0xF4;
    opTable->insert(pair<string, OpInfo*>("HIO", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "ADDR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0x90;
    opTable->insert(pair<string, OpInfo*>("ADDR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "CLEAR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xB4;
    opTable->insert(pair<string, OpInfo*>("CLEAR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "COMPR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xA0;
    opTable->insert(pair<string, OpInfo*>("COMPR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "DIVR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0x9C;
    opTable->insert(pair<string, OpInfo*>("DIVR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "MULR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0x98;
    opTable->insert(pair<string, OpInfo*>("MULR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "RMO";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xAC;
    opTable->insert(pair<string, OpInfo*>("RMO", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SHIFTL";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xA4;
    opTable->insert(pair<string, OpInfo*>("SHIFTL", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SHIFTR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xA8;
    opTable->insert(pair<string, OpInfo*>("SHIFTR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SUBR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0x94;
    opTable->insert(pair<string, OpInfo*>("SUBR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SVC";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xB0;
    opTable->insert(pair<string, OpInfo*>("SVC", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "TIXR";
    opTabMap->format = FORMAT_2;
    opTabMap->opCode = 0xB8;
    opTable->insert(pair<string, OpInfo*>("TIXR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "ADD";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x18;
    opTable->insert(pair<string, OpInfo*>("ADD", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "ADDF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x58;
    opTable->insert(pair<string, OpInfo*>("ADDF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "AND";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x40;
    opTable->insert(pair<string, OpInfo*>("AND", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "COMP";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x28;
    opTable->insert(pair<string, OpInfo*>("COMP", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "COMPF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x88;
    opTable->insert(pair<string, OpInfo*>("COMPF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "DIV";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x24;
    opTable->insert(pair<string, OpInfo*>("DIV", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "DIVF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x64;
    opTable->insert(pair<string, OpInfo*>("DIVF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "OR";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x44;
    opTable->insert(pair<string, OpInfo*>("OR", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "RD";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xD8;
    opTable->insert(pair<string, OpInfo*>("RD", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "RSUB";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x4C;
    opTable->insert(pair<string, OpInfo*>("RSUB", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SSK";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xEC;
    opTable->insert(pair<string, OpInfo*>("SSK", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STA";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x0C;
    opTable->insert(pair<string, OpInfo*>("STA", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STB";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x78;
    opTable->insert(pair<string, OpInfo*>("STB", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STCH";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x54;
    opTable->insert(pair<string, OpInfo*>("STCH", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x80;
    opTable->insert(pair<string, OpInfo*>("STF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STI";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xD4;
    opTable->insert(pair<string, OpInfo*>("STI", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STL";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x14;
    opTable->insert(pair<string, OpInfo*>("STL", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STS";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x7C;
    opTable->insert(pair<string, OpInfo*>("STS", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STSW";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xE8;
    opTable->insert(pair<string, OpInfo*>("STSW", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STT";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x84;
    opTable->insert(pair<string, OpInfo*>("STT", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "STX";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x10;
    opTable->insert(pair<string, OpInfo*>("STX", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SUB";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x1C;
    opTable->insert(pair<string, OpInfo*>("SUB", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "SUBF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x5C;
    opTable->insert(pair<string, OpInfo*>("SUBF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "TD";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xE0;
    opTable->insert(pair<string, OpInfo*>("TD", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "TIX";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x2C;
    opTable->insert(pair<string, OpInfo*>("TIX", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "WD";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xDC;
    opTable->insert(pair<string, OpInfo*>("WD", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "J";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x3C;
    opTable->insert(pair<string, OpInfo*>("J", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "JEQ";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x30;
    opTable->insert(pair<string, OpInfo*>("JEQ", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "JGT";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x34;
    opTable->insert(pair<string, OpInfo*>("JGT", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "JLT";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x38;
    opTable->insert(pair<string, OpInfo*>("JLT", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "JSUB";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x48;
    opTable->insert(pair<string, OpInfo*>("JSUB", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDA";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x00;
    opTable->insert(pair<string, OpInfo*>("LDA", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDB";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x68;
    opTable->insert(pair<string, OpInfo*>("LDB", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDCH";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x50;
    opTable->insert(pair<string, OpInfo*>("LDCH", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x70;
    opTable->insert(pair<string, OpInfo*>("LDF", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDL";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x08;
    opTable->insert(pair<string, OpInfo*>("LDL", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDS";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x6C;
    opTable->insert(pair<string, OpInfo*>("LDS", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDT";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x74;
    opTable->insert(pair<string, OpInfo*>("LDT", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LDX";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x04;
    opTable->insert(pair<string, OpInfo*>("LDX", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "LPS";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0xD0;
    opTable->insert(pair<string, OpInfo*>("LPS", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "MUL";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x20;
    opTable->insert(pair<string, OpInfo*>("MUL", opTabMap));

    opTabMap = new OpInfo;
    opTabMap->mnmoneic = "MULF";
    opTabMap->format = FORMAT_3_4;
    opTabMap->opCode = 0x60;
    opTable->insert(pair<string, OpInfo*>("MULF", opTabMap));

    opTabMap = NULL;
}

map<string, OpInfo*>* getOpTab() {
    if (opTable == NULL) {
        opTable = new map<string, OpInfo*>();
        setOpTab();
    }
    return opTable;
}

void deleteOpTab() {
    for (map<string, OpInfo*>::iterator it = opTable->begin();
            it != opTable->end(); ++it) {
        OpInfo* ptr = it->second;
        opTable->erase(it->first);
        delete ptr;
    }
    delete opTable;
}

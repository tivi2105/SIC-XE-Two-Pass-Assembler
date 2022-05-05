#include "IOHandler.h"

int counter = 0;

string createLine(int address, string label, string op, string operands) {
    int length;
    char spaceChar = SPACE_CHAR;
    char zeroChar = ZERO_CHAR;
    string line;
    string lineNumberStr = intToString(++counter, false);
    length = LINE_NUMBER_WIDTH;
    string addressStr = intToString(address, true);
    setLineLen(&lineNumberStr, length, spaceChar, false);
    length = 6;
    setLineLen(&addressStr, length, zeroChar, true);
    length = ADDRESS_WIDTH;
    setLineLen(&addressStr, length, spaceChar, false);
    length = LABEL_WIDTH;
    if (op.at(0) == '+')
        length -= 1;
    setLineLen(&label, length, spaceChar, false);
    length = MNEMONIC_WIDTH;
    if (op.at(0) == '+')
        length += 1;
    setLineLen(&op, length, spaceChar, false);
    length = OPERANDS_WIDTH;
    setLineLen(&operands, length, spaceChar, false);
    line = lineNumberStr + addressStr + label + op + operands;
    return line;
}

string createLineForPass2(int address, string label, string op_str,string operands, bitset<6> flags, string opCode) {
    int length;
    char spaceChar = SPACE_CHAR;
    char zeroChar = ZERO_CHAR;
    string line;
    string lineNumberStr = intToString(++counter, false);
    length = LINE_NUMBER_WIDTH;
    string addressStr = intToString(address, true);
    setLineLen(&lineNumberStr, length, spaceChar, false);
    length = 6;
    setLineLen(&addressStr, length, zeroChar, true);
    length = ADDRESS_WIDTH;
    setLineLen(&addressStr, length, spaceChar, false);
    length = LABEL_WIDTH;
    if (op_str.at(0) == '+')
        length -= 1;
    setLineLen(&label, length, spaceChar, false);
    length = MNEMONIC_WIDTH;
    if (op_str.at(0) == '+')
        length += 1;
    setLineLen(&op_str, length, spaceChar, false);
    length = OPERANDS_WIDTH;
    setLineLen(&operands, length, spaceChar, false);
    string flags_str = flags.to_string(), mod_flags_str = "";
    if (flags_str.compare("000000") != 0) {
        for (int i = 0; (unsigned) i < flags_str.size(); i++) {
            mod_flags_str += flags_str[i];
            if ((unsigned) i + 1 < flags_str.size())
                mod_flags_str += " ";
        }
    }
    length = FLAGS_WIDTH;
    setLineLen(&mod_flags_str, length, spaceChar, false);
    length = OPCODE_WIDTH;
    setLineLen(&opCode, length, spaceChar, false);
    line = lineNumberStr + addressStr + label + op_str + operands
            + mod_flags_str + opCode;
    return line;
}

void deleteFile(string name) {
    remove(name.c_str());

}

void createLine(ofstream* file, string line) {
    *file << line << endl;
}

string readLine(ifstream* file) {
    string line = "";
    getline(*file, line);
    return line;
}

void createTitle(ofstream* file) {

    int fileLineLength = FILE_LINE_WIDTH;
    int lineNumberLength = LINE_NUMBER_WIDTH;
    int addressLength = ADDRESS_WIDTH;
    int labelLength = LABEL_WIDTH;
    int mnemonicLength = MNEMONIC_WIDTH;
    int operandsLength = OPERANDS_WIDTH;
    char spaceChar = SPACE_CHAR;
    char equalChar = EQUAL_CHAR;
    string line = "";
    line = "Pass 1\n";
    createLine(file, line);
    line = "";
    string temp = "Line number";
    setLineLen(&temp, lineNumberLength, spaceChar, false);
    line += temp;
    temp = "Address";
    setLineLen(&temp, addressLength, spaceChar, false);
    line += temp;
    temp = "Label";
    setLineLen(&temp, labelLength, spaceChar, false);
    line += temp;
    temp = "Op";
    setLineLen(&temp, mnemonicLength, spaceChar, false);
    line += temp;
    temp = "Operands";
    setLineLen(&temp, operandsLength, spaceChar, false);
    line += temp;
    createLine(file, line);
    line = "";
    setLineLen(&line, fileLineLength, equalChar, false);
    createLine(file, line);

}

void createTitleForPass2(ofstream* file) {
    int pass2Length = PASS_2_WIDTH;
    int lineNumberLength = LINE_NUMBER_WIDTH;
    int addressLength = ADDRESS_WIDTH;
    int labelLength = LABEL_WIDTH;
    int mnemonicLength = MNEMONIC_WIDTH;
    int operandsLength = OPERANDS_WIDTH;
    int flagsLength = FLAGS_WIDTH;
    int opCodeLength = OPCODE_WIDTH;
    char spaceChar = SPACE_CHAR;
    char equalChar = EQUAL_CHAR;
    string line = "";
    line = "Pass 2\n";
    createLine(file, line);
    line = "";
    string temp = "Line number";
    setLineLen(&temp, lineNumberLength, spaceChar, false);
    line += temp;
    temp = "Address";
    setLineLen(&temp, addressLength, spaceChar, false);
    line += temp;
    temp = "Label";
    setLineLen(&temp, labelLength, spaceChar, false);
    line += temp;
    temp = "Op";
    setLineLen(&temp, mnemonicLength, spaceChar, false);
    line += temp;
    temp = "Operands";
    setLineLen(&temp, operandsLength, spaceChar, false);
    line += temp;
    temp = "n i x b p e";
    setLineLen(&temp, flagsLength, spaceChar, false);
    line += temp;
    temp = "Opcode";
    setLineLen(&temp, opCodeLength, spaceChar, false);
    line += temp;
    createLine(file, line);
    line = "";
    setLineLen(&line, pass2Length, equalChar, false);
    createLine(file, line);

}

void createError(ofstream* file, string error) {
    vector<string> vecOfStrs = split(error, '\n');

    for (unsigned int i = 0; i < vecOfStrs.size(); i++) {
        error = vecOfStrs.at(i);
        int errLength = error.length();
        int offset = 19;
        int excess;
        char spaceChar = SPACE_CHAR;
        char asterisk = ASTERISK_CHAR;

        setLineLen(&error, errLength + 3, asterisk, true);
        setLineLen(&error, errLength + offset, spaceChar, true);
        excess = 63 - errLength;

        setLineLen(&error, errLength + excess, spaceChar, false);
        createLine(file, error);
    }
}

void createBorder(ofstream* file) {
    char newLine = NEW_LINE_CHAR;
    int fileLineLength = FILE_LINE_WIDTH;
    string line = "";
    char asterickChar = ASTERISK_CHAR;
    for (int i = 0; i < fileLineLength; i++)
        line += asterickChar;
    createLine(file, newLine + line + newLine);
}

void createBorder_Pass2(ofstream* file) {
    char newLine = NEW_LINE_CHAR;
    int fileLineLength = PASS_2_WIDTH;
    string line = "";
    char asterickChar = ASTERISK_CHAR;
    for (int i = 0; i < fileLineLength; i++)
        line += asterickChar;
    createLine(file, newLine + line + newLine);
}

void createSymTab(Ui::MainWindow *ui, ofstream* file, map<string, symInfo*>* theMap) {

    if (theMap->empty())
        return;
    char spaceChar = SPACE_CHAR;
    char dashChar = DASH_CHAR;
    char equalChar = EQUAL_CHAR;
    char zeroChar = ZERO_CHAR;

    string temp, headerLine = "";
    string opst;
    ui->sym_tb->setText("");
    createLine(file, "\t Symbol\t\t Table\t\t (values in hex)\n");
    temp = "";
    setLineLen(&temp, 33, equalChar, false);
    createLine(file, temp);
    temp = "|\tname";
    setLineLen(&temp, 11, spaceChar, false);
    headerLine += temp;
    temp = "address";
    setLineLen(&temp, 10, spaceChar, false);
    headerLine += temp;
    temp = "Abs/Rel";
    temp += "\t|";
    headerLine += temp;
    createLine(file, headerLine);
    temp = "|\t";
    setLineLen(&temp, 28, dashChar, false);
    temp += "\t|";
    createLine(file, temp);
    for (map<string, symInfo*>::const_iterator it = theMap->begin(); it
            != theMap->end(); it++) {
        string symbol = "|\t" + it->first;
        symInfo* n = it->second;
        string address = intToString(n->address, true);
        string rel = "";
        if (n->relocatble)
            rel = "Rel";
        else
            rel = "Abs";
        setLineLen(&symbol, 11, spaceChar, false);
        string line = symbol;
        setLineLen(&address, 4, zeroChar, true);
        setLineLen(&address, 5, spaceChar, true);
        setLineLen(&address, 10, spaceChar, false);
        line += address;
        setLineLen(&address, 10, spaceChar, true);
        setLineLen(&rel, 7, spaceChar, true);
        rel += "\t|";
        line += rel;
        createLine(file, line);
        opst+= "\n" + line;

    }
    temp = "";
    ui->sym_tb->setText(opst.c_str());
    setLineLen(&temp, 33, equalChar, false);
    createLine(file, temp);
}

void createComment(ofstream* file, string comment) {
    int lineNumberLength = LINE_NUMBER_WIDTH;
    int addressLength = ADDRESS_WIDTH;
    char spaceChar = SPACE_CHAR;
    setLineLen(&comment,
            comment.size() + lineNumberLength + addressLength, spaceChar, true);
    createLine(file, comment);
}

void createTitleRecord(ofstream* file, string prog_name, int starting_add, int prog_length) {
    char spaceChar = SPACE_CHAR;
    char zeroChar = ZERO_CHAR;

    string line = "\t Header Record :\t\t\n";
    createLine(file, line);
    line = "H^";
    setLineLen(&line, 11, spaceChar, true);
    if (prog_name.compare("") == 0) setLineLen(&prog_name, 6, spaceChar, true);
    line += prog_name;
    line += "^";
    string starting_add_str = intToString(starting_add, true);
    setLineLen(&starting_add_str, 6, zeroChar, true);
    line += starting_add_str;
    line += "^";
    string prog_length_str = intToString(prog_length, true);
    setLineLen(&prog_length_str, 6, zeroChar, true);
    line += prog_length_str;
    createLine(file, line);
    line = "";
    setLineLen(&line, 11, spaceChar, true);
    line += "Program Name: ";
    if (prog_name.compare("") == 0) line += "Unknown.";
    else line += prog_name;
    createLine(file, line);
    line = "";
    setLineLen(&line, 11, spaceChar, true);
    line += "Loading Address: ";
    line += starting_add_str;
    line += "\n";
    createLine(file, line);

}

void createTextRecords(ofstream* file, vector<string>* opCodes, vector<locatedParsedLine>* lpl) {
    char zeroChar = ZERO_CHAR;
    char spaceChar = SPACE_CHAR;
    int start_address = 0;
    int end_address = 0;
    string start_address_str;
    string length_str;

    string line = "\tText Record(s) :\t\t\n";
    string temp_line = "";
    createLine(file, line);

    for (unsigned int i = 0; i< opCodes->size();){
        if ((*opCodes)[i].compare("") == 0 || (*lpl)[i].p.lable.compare("*") == 0){
                i++;
                continue;
        }

        line ="T^";
        setLineLen(&line, 11, spaceChar, true);
        start_address = (*lpl)[i].loc;
        start_address_str = intToString(start_address, true);
        setLineLen(&start_address_str, 6, zeroChar, true);
        line += start_address_str;
        int x = 0;
        string temp = "";
        while (1){
            if (i >= (*opCodes).size() || x >= 5)
                break;

            if ((*opCodes)[i].compare("") == 0 || (*lpl)[i].p.lable.compare("*") == 0){
                    i++;
                    break;
            }
            temp += "^";
            temp += (*opCodes)[i];
            if (i+1 < (*opCodes).size()) end_address = (*lpl)[i+1].loc;
            else end_address = (*lpl)[i].loc;
            i++;
            x++;
        }
        length_str = intToString(end_address - start_address, true);
        setLineLen(&length_str, 2, zeroChar, true);
        line += "^";
        line += length_str;
        line += temp;
        createLine(file, line);
    }
}

void createModRecords(ofstream* file, vector<modification>* mods) {
    char zeroChar = ZERO_CHAR;
    char spaceChar = SPACE_CHAR;
    int address = 0, hBytes = 0;
    string address_str, hBytes_str;
    string line = "\tModification Record(s) :\t\t\n";
    createLine(file, line);
    for (int i = 0; (unsigned)i < mods->size(); i++) {
        modification mod = (*mods)[i];
        if (mod.exRef) {
            for (int j = 0; (unsigned)j < mod.refs.size(); j++) {
                line = "M^";
                setLineLen(&line, 11, spaceChar, true);
                address = mod.address;
                address_str = intToString(address, true);
                line += address_str;
                line += "^";
                hBytes = mod.hbytes;
                hBytes_str = intToString(hBytes, false);
                setLineLen(&hBytes_str, 2, zeroChar, true);
                line += hBytes_str;
                line += "^";
                line += mod.signs[j];
                line += mod.refs[j];
                createLine(file, line);
            }
        }
        else {
            line = "M^";
            setLineLen(&line, 11, spaceChar, true);
            address = mod.address;
            address_str = intToString(address, true);
            line += address_str;
            line += "^";
            hBytes = mod.hbytes;
            hBytes_str = intToString(hBytes, false);
            setLineLen(&hBytes_str, 2, zeroChar, true);
            line += hBytes_str;
            createLine(file, line);
        }
    }
    createLine(file, "");
}

void createDefineRecord(ofstream* file, vector<string>* exdefs, map<string, symInfo*>* theMap) {
    char zeroChar = ZERO_CHAR;
    char spaceChar = SPACE_CHAR;

    int address = 0;
    int i = 0;
    string address_str;
    string line = "\tDefine Record :\t\t\n";
    createLine(file, line);
    for (; (unsigned)i < exdefs->size(); i++) {
        line = "D^";
        setLineLen(&line, 11, spaceChar, true);
        do {
            if ((unsigned)i >= exdefs->size()) break;
            string s = (*exdefs)[i];
            symInfo* sInfo = theMap->at(s);
            address = sInfo->address;
            address_str = intToString(address, true);
            setLineLen(&address_str, 6, zeroChar, true);
            line += (*exdefs)[i];
            line += "^";
            line += address_str;
            if ((i+1) % 5 != 0 && (unsigned)(i+1) < exdefs->size()) line += "^";
            i++;
        } while (i % 5 != 0);
        if ((unsigned)i >= exdefs->size())line += "\n";
        i--;
        createLine(file, line);
    }

}

void createReferRecord(ofstream* file, vector<string>* exrefs) {
    char spaceChar = SPACE_CHAR;
    int i = 0;

    string line = "\tRefer Record :\t\t\n";
    createLine(file, line);
    for (; (unsigned)i < exrefs->size(); i++) {
        line = "R^";
        setLineLen(&line, 11, spaceChar, true);
        do {
            if ((unsigned)i >= exrefs->size()) break;
            line += (*exrefs)[i];
            if ((i+1) % 5 != 0 && (unsigned)(i+1) < exrefs->size()) line += "^";
            i++;
        } while (i % 5 != 0);
        if ((unsigned)i >= exrefs->size()) line += "\n";
        i--;
        createLine(file, line);
    }
}

void createEndRecord() {

}

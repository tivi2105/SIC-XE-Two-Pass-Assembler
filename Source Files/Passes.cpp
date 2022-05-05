#include "Passes.h"
string errorMessage;
vector<transformLine> literalValue;
bool literalExistance;
int handelBYTE(string x) {
    string val = "";
    if (x.at(0) == 'x' || x.at(0) == 'X' || x.at(0) == 'c' || x.at(0) == 'C') {
        if (x.at(1) == '\'' && x.at(x.length() - 1) == '\'') {

            for (unsigned int i = 2; i < x.length() - 1; i++) {
                val += x.at(i);
            }
        } else {
            errorMessage += "\nnot a hexadecimal/character string";
            return 0;
        }
    } else {
        errorMessage += "\nillegal operand!!!";
        return 0;
    }

    if (x.at(0) == 'c' || x.at(0) == 'C')
        return val.length();

    return val.length() % 2 == 0 ? val.length() / 2 : val.length() / 2 + 1;
}

void addLableToSymTab(string x, metaData* curr) {
    if (x.compare("") != 0) {
        try {
            curr->symTab.at(x);
            errorMessage += "\nDuplicate Label Definition";
        } catch (out_of_range& oor) {
            symInfo* c = new symInfo;
            c->address = curr->loc_Ctr;
            c->relocatble = true;
            curr->symTab.insert(pair<string, symInfo*>(x, c));
        }
    }
}

void createFomatted(transformLine pl, metaData* current) {
    tocreate x;
    x.data = createLine((*current).loc_Ctr, pl.lable, pl.opcode,
            mergeStrings(pl.operand1, pl.operand2, ','));
    x.type = 1;
    current->dataSect.push_back(x);
    locatedParsedLine temp;
    temp.p = pl;
    temp.loc = current->loc_Ctr;
    current->locpl.push_back(temp);
}

void incLocCtr(metaData* cur, int incr) {
    cur->loc_Ctr += incr;
    if (cur->loc_Ctr > 0x33039) {
        errorMessage += "\nout of range address";
    }
}

void createAndIncr(int incr, transformLine pl, metaData* curr, bool* started,
        bool* ended) {
    if (!*started) {
        *started = true;
        curr->loc_Ctr = 0;
        createFomatted(pl, curr);
        addLableToSymTab(pl.lable, curr);
        incLocCtr(curr, incr);
    } else if (*started && !*ended) {
        createFomatted(pl, curr);
        addLableToSymTab(pl.lable, curr);
        incLocCtr(curr, incr);
    } else if (*ended) {
        errorMessage += "\nDefinition After END";
    }
}

bool hasExternalReference(transformLine pl, metaData* current, vector<string>* refs,
        vector<char>* signs) {
    if (pl.operand1.find("(") != string::npos
            && pl.operand1.find(")") != string::npos) {
        errorMessage += "\nBrackets are not allowed";
        return false;
    }

    vector<string> all = generalSplit(pl.operand1, "+-*/");
    bool answer = false;
    string myOperand = pl.operand1;
    for (unsigned int i = 0; i < all.size(); i++) {
        if (find(current->extRef.begin(), current->extRef.end(), all[i])
                != current->extRef.end()) {
            refs->push_back(all[i]);
            answer = true;
            int position = myOperand.find(all[i]);
            myOperand.substr(position + all[i].size());
            if (position == 0) {
                signs->push_back('+');
            } else {
                char temp = myOperand.at(position - 1);
                if (temp == '+' || temp == '-')
                    signs->push_back(temp);
            }
        }
    }
    return answer;
}

void handelModRecord(transformLine pl, metaData* current) {
    modification mod;
    bool hasP = trim(pl.opcode).at(0) == '+';
    bool hasExRef = hasExternalReference(pl, current, &(mod.refs),
            &(mod.signs));

    if (hasP) {
        mod.address = current->loc_Ctr + 1;
        mod.hbytes = 5;
        if (hasExRef) {
            mod.exRef = true;
        } else {
            mod.exRef = false;
        }
        current->mods.push_back(mod);
    } else {
        if (hasExRef) {
            mod.exRef = true;
            if (toUpperCase(pl.opcode).compare("BYTE") == 0) {
                mod.address = current->loc_Ctr;
                mod.hbytes = 2 * handelBYTE(pl.operand1);
                current->mods.push_back(mod);
            } else if (toUpperCase(pl.opcode).compare("WORD") == 0) {
                mod.address = current->loc_Ctr;
                mod.hbytes = 6;
                current->mods.push_back(mod);
            } else {
                errorMessage += "\nUsing ExtRef with no +";
            }
        }
    }
}
bool checkLabels(vector<string> labels, metaData* current) {
    if (labels.size() == 1 && labels[0] == "*")
        return true;
    for (unsigned int i = 0; i < labels.size(); i++) {
        if (labels[i].at(0) == '#' || labels[i].at(0) == '@')
            labels[i] = labels[i].substr(1);
        if (!isNumeric(labels[i])) {
            try {
                current->symTab.at(labels[i]);
            } catch (out_of_range& x) {
                return false;
            }
        }
    }
    return true;
}
void handleEquate(transformLine pl, metaData * current) {
    if (findExpressionType(pl.operand1) != -1) {

        vector<string> labels = generalSplit(pl.operand1, "+-*/");
        if (pl.operand1.compare("*") == 0)
            labels.push_back("*");
        if (!checkLabels(labels, current)) {
            errorMessage += "\nUNIDENTIFIED LABELS";
            return;
        } else {
            int value;
            try {
                value = evaluateOperand(pl.operand1, current);
            } catch (string* e) {
                errorMessage += "\nUnable to evaluate the given expression";
                delete e;
                return;
            }
            symInfo* c = new symInfo;
            c->address = value;
            c->relocatble = true;
            current->symTab.insert(pair<string, symInfo*>(pl.lable, c));
            createFomatted(pl, current);
        }
    } else {

        errorMessage += "\nInvalid Expression";
    }
}

void handleOrg(transformLine pl, metaData* current) {
    if (validateExpression(pl.operand1)) {
        vector<string> labels = generalSplit(pl.operand1, "+-*/");
        if (!checkLabels(labels, current)) {
            errorMessage += "\n UNIDENTIFIED LABELS";
            return;
        } else {
            int value;
            try {
                value = evaluateExpression(pl.operand1, current);
            } catch (string* e) {
                errorMessage += "\nUnable to evaluate the given expression";
                delete e;
                return;
            }
            createFomatted(pl, current);
            current->loc_Ctr = value;
        }
    } else {
        errorMessage += "\nInvalid Expression";
    }
}

bool isLit(string pl) {
    if (pl.at(0) == '=')
        return true;
    return false;
}

int literalLength(string literal, metaData* current) {
    int x;

    int y = evaluateOperand(literal, current);
    if (literal.at(1) == 'C') {
        string s = literal.substr(3, literal.length() - 4);
        x = s.length();
    } else if (literal.at(1) == 'X') {
        string s = literal.substr(3, literal.length() - 4);
        x = s.length() * 2;
    } else
        x = 3;
    return x;

}
void createliteralValue(metaData* current) {
    literalExistance = false;
    for (unsigned int i = 0; i < literalValue.size(); i++) {
        transformLine p;
        p.lable = "*";
        p.opcode = trim(literalValue[i].operand1);
        p.operand1 = "";
        p.operand2 = "";
        p.err = "";
        createFomatted(p, current);
        current->litTab.insert(
                pair<string, int>(trim(literalValue[i].operand1), current->loc_Ctr));
            //  Increment by the literal length
            incLocCtr(current, literalLength(literalValue[i].operand1, current));

    }
    literalValue.clear();
}

void addExtRefs(transformLine pl, metaData* current) {
    if (current->extRef.size() == 0) {
        current->extRef = generalSplit(pl.operand1, ",");
        for (unsigned int i = 0; i < current->extRef.size(); i++) {
            current->extRef[i] = trim(current->extRef[i]);
        }
    } else
        errorMessage +=
                "\nEXTREF can be written only once in a control section";
}

void addExtDefs(transformLine pl, metaData* current) {
    if (current->extDef.size() == 0) {
        current->extDef = generalSplit(pl.operand1, ",");
        for (unsigned int i = 0; i < current->extDef.size(); i++) {
            current->extDef[i] = trim(current->extDef[i]);
        }
    } else
        errorMessage +=
                "\nEXTDEF can be written only once in a control section";
}

void getlocCtrIncr(transformLine pl, metaData* current, bool* started, bool* ended) {
    int incr = validate(pl);
    if (pl.operand1.length() > 0 && isLit(pl.operand1)) {
        bool add = true;
        for(unsigned int y = 0; y < literalValue.size() ; y++)
        {
            if(literalValue[y].operand1.compare(pl.operand1)==0){
                add = false;
                break;
            }
        }
        if(add)
        {
            literalValue.push_back(pl);
        }
        literalExistance = true;
    }
    handelModRecord(pl, current);
    if (incr == -1) { // Directive
        string upper = toUpperCase(pl.opcode);
        if (upper.compare("START") == 0) {
            if (!*started) {
                *started = true;
                incLocCtr(current, parseHexaDecimal(pl.operand1));
                createFomatted(pl, current);
                addLableToSymTab(pl.lable, current);
            } else {
                errorMessage += "\nMisplaced START";
            }
        } else if (upper.compare("END") == 0) {
            if (!*started) {
                errorMessage += "\nEnded before START";
            } else if (*started && !*ended) {
                *ended = true;
                incLocCtr(current, 0);
                createFomatted(pl, current);
                addLableToSymTab(pl.lable, current);
            } else if (*ended) {
                errorMessage += "\nMultiple END clauses.";
            }
        } else if (upper.compare("BYTE") == 0) {
            createAndIncr(handelBYTE(pl.operand1), pl, current, started, ended);
        } else if (upper.compare("WORD") == 0) {
            createAndIncr(3, pl, current, started, ended);
        } else if (upper.compare("RESB") == 0) {
            createAndIncr(parseDecimal(pl.operand1), pl, current, started,
                    ended);
        } else if (upper.compare("RESW") == 0) {
            createAndIncr(parseDecimal(pl.operand1) * 3, pl, current, started,
                    ended);
        } else if (upper.compare("EQU") == 0) {
            handleEquate(pl, current);
        } else if (upper.compare("ORG") == 0) {
            handleOrg(pl, current);
        } else if (upper.compare("LTORG") == 0) {
            createFomatted(pl, current);
            createliteralValue(current);
        } else if (upper.compare("EXTREF") == 0) {
            createFomatted(pl, current);
            addExtRefs(pl, current);
        } else if (upper.compare("EXTDEF") == 0) {
            createFomatted(pl, current);
            addExtDefs(pl, current);
        }
    } else { //Operations (formats)
        if (!*started) {
            *started = true;
            current->loc_Ctr = 0;
            createFomatted(pl, current);
            addLableToSymTab(pl.lable, current);
            incLocCtr(current, incr);
        } else if (*started && !*ended) {
            createFomatted(pl, current);
            addLableToSymTab(pl.lable, current);
            incLocCtr(current, incr);
        } else if (*ended) {
            errorMessage += "\nDefinition After END";
        }
    }
}

bool isComment(transformLine pl) {
    return trim(pl.opcode).at(0) == '.';
}

bool isEmptyLine(transformLine pl) {
    if (pl.lable.compare("") == 0 && pl.opcode.compare("") == 0
            && pl.operand1.compare("") == 0 && pl.operand2.compare("") == 0)
        return true;

    return false;
}

metaData runPass1(vector<transformLine> v, bool* started, bool* ended,
        bool* successfull) {
    transformLine pl;
    metaData current;
    current.loc_Ctr = 0;
    literalValue.clear();
    literalExistance = false;
    for (unsigned int i = 0; i < v.size(); i++) {
        pl = v[i];
        if (isEmptyLine(pl)) {
            continue;
        }
        if (isComment(pl)) {
            tocreate x;
            x.data = pl.opcode;
            x.type = 0;
            current.dataSect.push_back(x);
            continue;
        }
        try {
            getlocCtrIncr(pl, &current, started, ended);
        } catch (string* e) {
            createFomatted(pl, &current);
            *(successfull) = false;
            errorMessage += "\n";
            errorMessage += *e;
            delete e;
        }
        if (errorMessage.compare("") != 0) {
            tocreate x;
            *successfull = false;
            x.data = errorMessage;
            x.type = -1;
            current.dataSect.push_back(x);
            errorMessage = "";
        }
    }
    if (literalExistance)
        createliteralValue(&current);
    return current;
}

void runPass2(Ui::MainWindow *ui, ofstream* opFile, vector<metaData>* cSects) {

    errorMessages = "";
    createBorder_Pass2(opFile);
    createTitleForPass2(opFile);
    vector<vector<string> > objectCodes;
    string opst;
    ui->passII_tb->setText("");
    bool success = true;
    int starting = cSects->at(0).locpl.at(0).loc;
    metaData cc = cSects->at(cSects->size() - 1);
    int ending = cc.locpl.at(cc.locpl.size() - 1).loc;
    for (unsigned int i = 0; i < cSects->size(); i++) {
        vector<string> ii;
        objectCodes.push_back(ii);
        vector<locatedParsedLine> v = cSects->at(i).locpl;
        for (unsigned j = 0; j < v.size() - 1; j++) {
            opCode x;
            string objectCode = "";
            try {
                x = getOpCode(v[j].p, v[j + 1].loc, &(cSects->at(i)));
                objectCode = evaluateOpcode(x);
            } catch (string* err) {
                errorMessages += "\n";
                errorMessages += *err;
                success = false;
                delete err;
            } catch (string& err) {
                errorMessages += "\n";
                errorMessages += err;
                success = false;
            }

            string operands = mergeStrings(v[j].p.operand1, v[j].p.operand2,
                    ',');
            string line = createLineForPass2(v[j].loc, v[j].p.lable,
                    v[j].p.opcode, operands, x.flags, objectCode);

            objectCodes[i].push_back(objectCode);
            createLine(opFile, line);
            opst+= "\n" + line;
            if (errorMessages.compare("") != 0) {
                createError(opFile, errorMessages);
                errorMessages = "";
            }
        }
    }

    ui->passII_tb->setText(opst.c_str());

    if (!success)
        return;

    opFile->close();

    string out2 = "objectFile.txt";
    deleteFile(out2.c_str());
    opFile->open(out2.c_str(), std::ios::app);

    for (unsigned int i = 0; i < cSects->size(); i++) {
        createTitleRecord(opFile, "", starting, ending - starting);
        createDefineRecord(opFile, &(cSects->at(i).extDef),
                &(cSects->at(i).symTab));
        createReferRecord(opFile, &(cSects->at(i).extRef));
        createTextRecords(opFile, &(objectCodes[i]), &(cSects->at(i).locpl));
        createModRecords(opFile, &(cSects->at(i).mods));
        createBorder_Pass2(opFile);
    }

    return;
}

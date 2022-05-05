#include "Operations.h"

metaData* curMetaData;

void setCurMetaData(metaData* x) {
    curMetaData = x;
}

int getValueOf(string name) {
    if (name.compare("*") == 0) {
        return curMetaData->loc_Ctr;
    }

    try {
        return curMetaData->symTab.at(name)->address;
    } catch (out_of_range& orr) {
        if (find(curMetaData->extRef.begin(), curMetaData->extRef.end(), name)
                != curMetaData->extRef.end()) {
            return 0;
        } else {
            throw(string) "label not found";
        }
    }
}

bool isNum(string num) {
    unsigned int i = 0;
    if (num[0] == '-')
        i++;
    for (; i < num.length(); i++)
        if (num[i] < '0' || num[i] > '9')
            return false;
    return true;
}

bool isOp(string operand) {
    if(operand.compare("*")==0)
        return true;
    for (unsigned int i = 0; i < operand.length(); i++)
        if ((operand[i] < 'A' || operand[i] > 'Z')
                && (operand[i] < 'a' || operand[i] > 'z'))
            return false;
    return true;
}

bool isHex(string op) {
    if (op.length() < 3 || op[0] != 'X' || op[1] != '\''
            || op[op.length() - 1] != '\'')
        return false;
    return isHexa(op.substr(2, op.length() - 3));
}

bool isByte(string op) {
    if (op.length() < 3 || op[0] != 'C' || op[1] != '\''
            || op[op.length() - 1] != '\'')
        return false;
    return true;
}

bool isImmediateOp(string op) {
    if (op.length() < 2 || op[0] != '#')
        return false;
    string rest = op.substr(1);
    return isNum(rest) || isOp(rest) || isHex(rest)
            || isByte(rest);
}

bool isIndirectOp(string op) {
    if (op.length() < 2 || op[0] != '@')
        return false;
    string rest = op.substr(1);
    return isNum(rest) || isOp(rest);
}

bool isLiteralOp(string op) {
    if (op.length() < 2 || op[0] != '=')
        return false;
    string rest = op.substr(1);
    return isNum(rest) || isOp(rest) || isHex(rest)
            || isByte(rest);
}

int findExpressionType(string op) {
    if (op.length() == 0)
        return -1;
    if (isImmediateOp(op))
        return 0;
    if (isIndirectOp(op))
        return 1;
    if (isLiteralOp(op))
        return 2;
    if (isHex(op))
        return 3;
    if (isByte(op))
        return 4;
    if (isNum(op))
        return 5;
    if (validateExpression(op))
        return 6;
    return -1;
}

int valueOfByteConstant(string op) {
    int value = 0;
    for (unsigned int i = 0; i < op.length(); i++)
        value += ((int) op[i]) << (i * 8);
    return value;
}

int valueOfImmediateOperand(string op) {
    string rest = op.substr(1);
    if (isHex(rest))
        return parseHexaDecimal(rest.substr(2, rest.length() - 3));
    if (isByte(rest))
        return valueOfByteConstant(rest.substr(2, rest.length() - 3));
    if (isOp(rest))
        return getValueOf(rest);
    if (isNum(rest))
        return parseDecimal(rest);

    throw(string) "invalid immediate operand";
}

int valueOfIndirectOperand(string op) {
    string rest = op.substr(1);
    if (isOp(rest))
        return getValueOf(rest);
    if (isNum(rest))
        return parseDecimal(rest);
    throw(string) "invalid indirect operand";
}

int valueOfLiteralOperand(string op) {

    string rest = op.substr(1);
    if (isHex(rest))
        return parseHexaDecimal(rest.substr(2, rest.length() - 3));
    if (isByte(rest))
        return valueOfByteConstant(rest.substr(2, rest.length() - 3));
    if (isNum(rest))
        return parseDecimal(rest);

    throw(string) "invalid literal operand";
}

int addressOfLiteralOperand(string op) {
    try
    {
        return curMetaData->litTab.at(op);
    } catch(out_of_range& err){
        throw(string) "invalid literal operand";
    }
}

bool isNoOperands(string s) {
    s=toUpperCase(s);
    return s.compare("RSUB") == 0 || s.compare("NOBASE") == 0
            || s.compare("HIO") == 0 || s.compare("FIX") == 0
            || s.compare("NORM") == 0 || s.compare("TIO") == 0 || s.compare("START") == 0
            || s.compare("SIO") == 0 || s.compare("FLOAT") == 0||s.compare("CSECT") == 0;
}

transformLine parse(string line) {
    unsigned int i;
    transformLine pl;
    pl.lable = "";
    pl.opcode = "";
    pl.operand1 = "";
    pl.operand2 = "";

    string s[5] = { "", "", "", "", "" };
    i = 0;
    for (unsigned int j = 0; j < 5 && i < line.length(); j++) {
        for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);
        for (; i < line.length() && (line[i] != ' ' && line[i] != '\t'); i++)
            s[j] += line[i];

        if(j==0){
            string temp = toUpperCase(s[0]);
            if(temp.compare("EXTREF")==0 || temp.compare("EXTDEF") == 0){
                pl.opcode = s[0];
                for(int k=i; k<line.length(); k++)
                        pl.operand1 += line[k];
                return pl;
            }
        }else if(j==1){
            string temp = toUpperCase(s[1]);
            if(temp.compare("EXTREF")==0 || temp.compare("EXTDEF") == 0){
                pl.lable = s[0];
                pl.opcode = s[1];
                for(int k=i; k<line.length(); k++)
                        pl.operand1 += line[k];
                return pl;
            }
        }

    }
    for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);

    if(s[0] != "" && s[0][0] == '.'){
        for (i=0; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);
        for (; i < line.length(); i++)
            pl.opcode += line[i];
        return pl;
    }

    if(i != line.length())
        throw new string("Too Many Arguments");

    if (s[0] == "")
        return pl;
    else if (s[1] == "")
        pl.opcode = s[0];
    else if (s[2] == "") {
        if (isNoOperands(s[1])) {
            pl.lable = s[0];
            pl.opcode = s[1];
        } else {
            pl.opcode = s[0];
            pl.operand1 = s[1];
        }
    } else if (s[3] == "") {
        if (s[1][s[1].length() - 1] == ',' || s[2][0] == ',') {
            pl.opcode = s[0];
            pl.operand1 = s[1] + s[2];
        } else {
            pl.lable = s[0];
            pl.opcode = s[1];
            pl.operand1 = s[2];
        }
    } else if (s[4] == "") {
        if (s[2].compare(",") == 0) {
            pl.opcode = s[0];
            pl.operand1 = s[1] + s[2] + s[3];
        } else {
            pl.lable = s[0];
            pl.opcode = s[1];
            pl.operand1 = s[2] + s[3];
        }
    } else {
        if(s[3].compare(",") != 0)
            throw new string("Too Many Arguments");
        pl.lable = s[0];
        pl.opcode = s[1];
        pl.operand1 = s[2] + s[3] + s[4];
    }

    s[0] = "";
    s[1] = "";
    for (i = 0; i < pl.operand1.length() && pl.operand1[i] != ','; i++)
        s[0] += pl.operand1[i];
    if(i == pl.operand1.length()-1 && pl.operand1[i] == ',')
        s[1] = ",";
    else
        for (i++; i < pl.operand1.length(); i++)
            s[1] += pl.operand1[i];
    pl.operand1 = s[0];
    pl.operand2 = s[1];
    return pl;
}

int evaluateOperand(string operand, metaData* x) {
    int type = findExpressionType(operand);
    setCurMetaData(x);
    switch (type) {
    case 0: // #
        return valueOfImmediateOperand(operand);
    case 1: // @
        return valueOfIndirectOperand(operand);
    case 2: // =
        return valueOfLiteralOperand(operand);
    case 3:	// Hex Constant
        return parseHexaDecimal(operand.substr(2, operand.length() - 3));
    case 4:	// Byte Constant
        return valueOfByteConstant(operand.substr(2, operand.length() - 3));
    case 5:	// int constant
        return parseDecimal(operand);
    case 6: // expression
        return evaluateExpression(operand, curMetaData, false);
    default:
        throw(string) "invalid operand";
    }
}

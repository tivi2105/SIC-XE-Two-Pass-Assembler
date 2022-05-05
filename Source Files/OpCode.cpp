#include "OpCode.h"

map<string, unsigned char>* registers = NULL;

map<string, unsigned char>* getRegMap() {
    if (registers != NULL)
        return registers;

    registers = new map<string, unsigned char>();
    registers->insert(pair<string, unsigned char>("A", 0));
    registers->insert(pair<string, unsigned char>("X", 1));
    registers->insert(pair<string, unsigned char>("L", 2));
    registers->insert(pair<string, unsigned char>("B", 3));
    registers->insert(pair<string, unsigned char>("S", 4));
    registers->insert(pair<string, unsigned char>("T", 5));
    registers->insert(pair<string, unsigned char>("F", 6));
    return registers;
}

opCode getOpCode(transformLine pl, int loc_ctr, metaData* current) {
    opCode setReg;
    string modOp = (pl.opcode).at(0) == '+' ? (pl.opcode).substr(1) : pl.opcode;
    modOp = toUpperCase(modOp);
    string op1 = toUpperCase(pl.operand1);
    string op2 = toUpperCase(pl.operand2);

    OpInfo* opTabMap;
    try {
        opTabMap = getOpTab()->at(modOp);
    } catch (out_of_range& orr) {
        int value = 0;
        if (modOp.compare("WORD") == 0) {
            setReg.format = 11;

            value = evaluateOperand(pl.operand1, current);

            if (value >= -(1 << 23) && value <= (1 << 23) - 1) {
                setReg.displacement = value;
            } else {
                throw new string("Too large to store");
            }
        } else if (modOp.compare("BYTE") == 0) {
            setReg.format = 22;

            if (op1.at(0) == 'C') {
                if (op1.size() <= 10) {
                    setReg.displacement = evaluateOperand(pl.operand1, current);
                } else {
                    throw new string("Too large to store");
                }
            } else if (op1.at(0) == 'X') {
                if (op1.size() <= 17) {
                    setReg.displacement = evaluateOperand(pl.operand1, current);
                } else {
                    throw new string("Too large to store");
                }
            }
        } else if (modOp.at(0) == '=') {
            setReg.format = 22;
            setReg.displacement = valueOfLiteralOperand(pl.opcode);
        } else {
            setReg.format = 33;
        }
        return setReg;
    }

    unsigned char format = opTabMap->format;

    if (format == FORMAT_1) {
        setReg.operation = opTabMap->opCode;
        setReg.displacement = 0;
        setReg.format = FORMAT_1;
        setReg.flags[5] = 1;
        setReg.flags[4] = 1;
        setReg.flags[3] = 0;
        setReg.flags[2] = 0;
        setReg.flags[1] = 0;
        setReg.flags[0] = 0;
    } else if (format == FORMAT_2) {
        setReg.operation = opTabMap->opCode;
        setReg.reg1 = getRegMap()->at(op1);
        setReg.reg2 = (op2).compare("") != 0 ? registers->at(op2) : 0;
        setReg.format = FORMAT_2;
        setReg.flags[5] = 1;
        setReg.flags[4] = 1;
        setReg.flags[3] = 0;
        setReg.flags[2] = 0;
        setReg.flags[1] = 0;
        setReg.flags[0] = 0;
    } else {
        setReg.operation = opTabMap->opCode;
        setReg.format = FORMAT_3_4;

        if (modOp.compare("RSUB") == 0) {
            setReg.displacement = 0;
            setReg.flags[5] = 1; //n
            setReg.flags[4] = 1; //i
            setReg.flags[3] = 0; //x
            return setReg;
        }
        setReg.flags[0] = (pl.opcode).at(0) == '+' ? 1 : 0; //e

        if (op1.at(0) == '#') {
            setReg.flags[5] = 0; //n
            setReg.flags[4] = 1; //i
            setReg.flags[3] = 0; //x
        } else if ((pl.operand1).at(0) == '@') {
            setReg.flags[5] = 1; //n
            setReg.flags[4] = 0; //i
            setReg.flags[3] = 0; //x
        } else {
            setReg.flags[5] = 1; //n
            setReg.flags[4] = 1; //i
            setReg.flags[3] = (op2).compare("X") == 0 ? 1 : 0; //x
        }

        int address = 0;
        try {
            if (findExpressionType(pl.operand1) == 2)
                address = addressOfLiteralOperand(pl.operand1);
            else
                address = evaluateOperand(pl.operand1, current);
        } catch (string& err) {
            throw err;
        }

        if (setReg.flags[0]) {
            setReg.flags[2] = 0; //b
            setReg.flags[1] = 0; //p
            if (address >= -(1 << 19) && address <= (1 << 19) - 1)
                setReg.displacement = address;
            else
                throw new string("Too large to store");
        } else { //only 12 bits
            setReg.flags[2] = 0; //b
            setReg.flags[1] = 1; //p
            int disp = 0;
            if (findExpressionType(pl.operand1) == 6
                    || findExpressionType(pl.operand1) == 1
                    || findExpressionType(pl.operand1) == 2
                    || (findExpressionType(pl.operand1) == 0
                            && isOp(pl.operand1)))
                disp = address - loc_ctr;
            else
                disp = address;
            if (disp >= -(1 << 11) && disp <= (1 << 11) - 1) {
                setReg.displacement = disp;
            } else {
                throw new string("Too large to store");
            }
        }
    }
    return setReg;
}

string evaluateOpcode(opCode op) {
    string returnedString = "";
    if (op.format == 11) {
        string set_opcode = intToString(op.displacement, true);
        setLineLen(&set_opcode, 6, '0', true);
        return set_opcode;
    } else if (op.format == 22) {
        string set_opcode = intToString(op.displacement, true);
        setLineLen(&set_opcode, 2, '0', true);
        return set_opcode;
    } else if (op.format == 33) {
        return returnedString;
    } else if (op.format == FORMAT_1) {
        returnedString = intToString(op.operation, true);
    } else if (op.format == FORMAT_2) {
        returnedString = ((intToString(op.operation, true)
                + intToString(op.reg1, false)) + intToString(op.reg2, false));
    } else {
        int ni_val = 0, xbpe_val = 0;
        castBinaryToDecimal(op.flags, &ni_val, &xbpe_val);
        string disp_str = intToString(op.displacement, true);
        if (op.flags[0] == 0) {
            setLineLen(&disp_str, 3, '0', true);
            disp_str = disp_str.substr(disp_str.length() - 3);
        } else {
            setLineLen(&disp_str, 5, '0', true);
            disp_str = disp_str.substr(disp_str.length() - 5);
        }
        returnedString = ((intToString(op.operation + ni_val, true)
                + intToString(xbpe_val, true)) + disp_str);
    }
    return returnedString;
}

void castBinaryToDecimal(bitset<6> flags, int* ni_val, int* xbpe_val) {
    bitset<4> xbpe_flags;
    for (int i = 0; i < 4; i++)
        xbpe_flags[i] = flags[i];

    bitset<2> ni_flags;
    for (int i = 0; i < 2; i++)
        ni_flags[i] = flags[i + 4];

    *ni_val = *xbpe_val = 0;
    stringstream ss_1, ss_2;
    ss_1 << uppercase << std::hex << ni_flags.to_ulong();
    ss_1 >> std::hex >> *ni_val;
    ss_2 << std::hex << uppercase << xbpe_flags.to_ulong();
    ss_2 >> std::hex >> *xbpe_val;
}

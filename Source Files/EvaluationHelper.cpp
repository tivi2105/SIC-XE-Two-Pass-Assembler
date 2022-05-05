#include "EvaluationHelper.h"

metaData* metadata = NULL;

void setMetaData(metaData* x) {
    metadata = x;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_loc = str.find(from);
    if (start_loc == std::string::npos)
        return false;
    str.replace(start_loc, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from,const std::string& to) {
    if (from.empty())
        return;
    size_t start_loc = 0;
    while ((start_loc = str.find(from, start_loc)) != std::string::npos) {
        str.replace(start_loc, from.length(), to);
        start_loc += to.length();
    }
}

bool isNumber(string num) {
    unsigned int i = 0;
    if (num[0] == '-')
        i++;
    for (; i < num.length(); i++)
        if (num[i] < '0' || num[i] > '9')
            return false;
    return true;
}

bool isOpName(string operand) {
    for (unsigned int i = 0; i < operand.length(); i++)
        if ((operand[i] < 'A' || operand[i] > 'Z')
                && (operand[i] < 'a' || operand[i] > 'z'))
            return false;
    return true;
}

bool isOperator(char c) {
    return c == '+' || c == '/' || c == '*' || c == '-' || c == '%' || c == '(' || c == ')';
}

string valueOf(string name, bool placeHolderVals = false) {
    if (name[0] >= '0' && name[0] <= '9')
        return name;
    if (placeHolderVals)
        return "10";

    if(name.compare("*")==0){
        return intToString(metadata->loc_Ctr,false);
    }

    try {
        return intToString(metadata->symTab.at(name)->address, false);
    } catch (out_of_range& orr) {
        if (find(metadata->extRef.begin(), metadata->extRef.end(), name)
                != metadata->extRef.end()) {
            return "0";
        } else {
            throw(string) "label not found";
        }
    }
}

string substituteValues(string expression, bool placeHolderVals = false) {
    string exp = "";
    string name = "";
    bool nextCouldBeAsterix = true;

    for (unsigned int i = 0; i < expression.length();) {
        name = "";

        while (i < expression.length() && !isOperator(expression[i]))
            name += expression[i++];

        if( name.length() == 0 && nextCouldBeAsterix && expression[i] == '*')
            name += expression[i++];

        if (name.compare("") != 0)
            exp += valueOf(name, placeHolderVals);

        nextCouldBeAsterix = false;
        while (i < expression.length() && isOperator(expression[i])) {
            if (nextCouldBeAsterix && expression[i] == '*')
                exp += valueOf("*", placeHolderVals);
            else
                exp += expression[i];

            if (expression[i] != ')' && (!nextCouldBeAsterix || expression[i] != '*'))
                nextCouldBeAsterix = true;
            else
                nextCouldBeAsterix = false;
            i++;
        }
    }
    return exp;
}

int bodmas(char opr1, char opr2) {
    if (opr1 == opr2)
        return 0;
    if (opr1 == ')')
        return -1;
    if (opr2 == ')')
        return 1;
    if (opr1 == '*' || opr1 == '/' || opr1 == '%')
        return 1;
    if (opr2 == '*' || opr2 == '/' || opr2 == '%')
        return -1;
    if (opr1 == '+' || opr1 == '-')
        return 1;
    if (opr2 == '+' || opr2 == '-')
        return -1;
    if (opr2 == '(')
        return -1;
    if (opr1 == '(')
        return 1;
    return 0;
}

float evaluateExpressionsStd(float op1, char operation, float op2) {
    switch (operation) {
    case '*':
        return op1 * op2;
    case '/':
        return op1 / op2;
    case '+':
        return op1 + op2;
    case '-':
        return op1 - op2;
    case '%':
        return (int) op1 % (int) op2;
    }
    return false;
}

int evalAddressDegree(int relDeg1, int val1, char operation,
        int relDeg2, int val2) {
    switch (operation) {
    case '*':
        if (relDeg1 != 0 && relDeg2 != 0)
            throw(string) "Error multiplying relative addresses";
        if (relDeg1 == 0 && relDeg2 == 0)
            return 0;
        return (relDeg1 != 0) ? relDeg1 * val2 : relDeg2 * val1;
    case '/':
        if (relDeg1 != 0 || relDeg2 != 0)
            throw(string) "Error deviding relative addresses";
        return 0;
    case '+':
        return relDeg1 + relDeg2;
    case '-':
        return relDeg1 - relDeg2;
    case '%':
        if (relDeg1 != 0 || relDeg2 != 0)
            throw(string) "Error taking modulus of relative addresses";
        return 0;
    }
    return 0;
}

int valueExpression(string exp, queue<char> isRelative,bool *retFlagIsRelative) {
    stack<char> operations;
    stack<int> values, degreeOfRelativity;

    string op = "";
    unsigned int i;
    bool lastAddedIsOperand = false;
    for (i = 0; i < exp.length();) {
        op = "";
        while ((i < exp.length() && !isOperator(exp[i]))
                || (exp[i] == '-' && op.length() == 0 && !lastAddedIsOperand))
            op += exp[i++];
        if (op.compare("-") == 0) {
            values.push(-1);
            degreeOfRelativity.push(0);
            operations.push('*');
            lastAddedIsOperand = false;

        } else if (op.length() != 0) {
            values.push(atoi(op.c_str()));
            degreeOfRelativity.push((isRelative.front() == 'R') ? 1 : 0);
            isRelative.pop();
            lastAddedIsOperand = true;
        }

        else if (operations.empty() || exp[i] == '('
                || bodmas(operations.top(), exp[i]) != 1) {
            operations.push(exp[i++]);
            lastAddedIsOperand = false;
        } else
        {
            if (operations.top() == '(' && exp[i] == ')') {
                operations.pop();
                i++;
            } else {
                int op2 = values.top();
                values.pop();
                int rd2 = degreeOfRelativity.top();
                degreeOfRelativity.pop();
                int op1 = values.top();
                values.pop();
                int rd1 = degreeOfRelativity.top();
                degreeOfRelativity.pop();
                int result = evaluateExpressionsStd(op1, operations.top(), op2);
                degreeOfRelativity.push(
                        evalAddressDegree(rd1, op1, operations.top(),
                                rd2, op2));
                operations.pop();
                values.push(result);
            }
        }
    }
    while (operations.size() != 0) {
        int op2 = values.top();
        values.pop();
        int rd2 = degreeOfRelativity.top();
        degreeOfRelativity.pop();
        int op1 = values.top();
        values.pop();
        int rd1 = degreeOfRelativity.top();
        degreeOfRelativity.pop();
        int result = evaluateExpressionsStd(op1, operations.top(), op2);
        degreeOfRelativity.push(
                evalAddressDegree(rd1, op1, operations.top(), rd2,
                        op2));
        operations.pop();
        values.push(result);
    }

    if (values.size() == 1) {
        if (degreeOfRelativity.top() != 1 && degreeOfRelativity.top() != 0)
            throw(string) "invalid operations on relative variables";
        *retFlagIsRelative = (degreeOfRelativity.top() == 0) ? false : true;
        int ret = values.top();
        values.pop();
        return ret;
    }

    throw(string) "error evaluating expression";
}

queue<char> validate(string expression) {
    queue<char> exp;
    string operand = "";
    bool isNextOperand = true;
    int bracesCount = 0;

    for (unsigned int i = 0; i < expression.length();) {
        operand = "";
        while (i < expression.length() && !isOperator(expression[i]))
            operand += expression[i++];

        if (operand.length() > 0) {
            if (!isNextOperand)
                throw(string) "Invalid operations in expression1";
            if (isNumber(operand))
                exp.push('A');
            else if (isOpName(operand))
                exp.push('R');
            else
                throw(string) "Incorrect Operand Name";
            isNextOperand = false;
        }

        while (i < expression.length() && isOperator(expression[i])) {
            if (isNextOperand && expression[i] == '-')
                break;
            if (isNextOperand) {
                if (expression[i] == '*') {
                    exp.push('R');
                    isNextOperand = false;
                } else if (expression[i] == '(')
                    isNextOperand = true;
                else
                    throw(string) "Invalid operations in expression";
            } else {
                if (expression[i] != ')')
                    isNextOperand = true;
                else
                    isNextOperand = false;
            }
            if (expression[i] == '(')
                bracesCount++;
            if (expression[i] == ')')
                bracesCount--;
            if (bracesCount < 0)
                throw(string) "non-matching brackets";
            i++;
        }
    }
    if (isNextOperand)
        throw(string) "missing operand";
    if (bracesCount != 0)
        throw(string) "non-matching brackets";

    return exp;
}

int evaluateExpression(string exp, metaData* cs, bool placeHolderVals) {
    bool x;
    setMetaData(cs);
    return valueExpression(substituteValues(exp, placeHolderVals), validate(exp), &x);
}

bool validateExpression(string exp) {
    try {
        evaluateExpression(exp, metadata, true);
    } catch (string& s) {
        return false;
    }
    return true;
}

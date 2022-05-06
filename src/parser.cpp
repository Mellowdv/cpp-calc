#include "parser.h"
#include "tokens.h"
#include "variables.h"
#include <iostream>
#include <cmath>

int calculateFactorial(int n);

double statement() {
    Token t = ts.get();
    switch (t.type) {
    case let:
        return declaration();
    case declareConst:
        return declaration(newConst);
    case squareRoot: {
        return handleSqrt();
    }
    case power: {
        return handlePow();
    }
    default:
        ts.putback(t);
        return second_order();
    }
}

double primary()
{
    double result;
    std::string varName;
    Token t = ts.get();

    switch (t.type) {
    case number: 
        return t.value;
    case name:
        return handleNameTokens(t);
    case '+':
        return primary();
    case '-':
        return -primary();
    case '(':
        return handleBrackets(t);
    case '{':
        return handleBrackets(t);
    case squareRoot:
        return handleSqrt();
    case power:
        return handlePow();
    default:
        throw std::runtime_error("Primary expected.");
    }
}

double factorial() {

    double lhs = primary();
    Token t = ts.get();
    if (t.type == '!') {
        lhs = calculateFactorial(lhs);
        return lhs;
    }
    else {
        ts.putback(t);
        return lhs;
    }
}

double second_order()
{
    double lhs = first_order();
    Token t = ts.get();

    while (true) {
        if (t.type == '+') {
            lhs += first_order();
            t = ts.get();
        }
        else if (t.type == '-') {
            lhs -= first_order();
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

double first_order()
{
    double lhs = factorial();
    Token t = ts.get();

    while (true) {
        if (t.type == '*') {
            lhs *= factorial();
            t = ts.get();
        }
        else if (t.type == '/') {
            lhs = handleDivision(t, lhs);
            t = ts.get();
        }
        else if (t.type == '%') {
            lhs = handleModulo(t, lhs);
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

double declaration(bool isConst) {
    Token t = ts.get();
    if (t.type != name) throw std::runtime_error("Expected a name.");
    std::string varName = t.name;

    Token t2 = ts.get();
    if (t2.type != '=') throw std::runtime_error("Expected assignement.");
    
    double d = second_order();
    if (isConst)
        varTable.defineName(varName, d, isConst);
    else
        varTable.defineName(varName, d);
    return d;
}

Token handleStringInput(char& input) {
    if (isalpha(input)) {
        std::string s;
        s += input;
        while (std::cin.get(input) && (isalpha(input) || isdigit(input) || input == '_')) s += input;
        std::cin.putback(input);
        if (s == declKey) return Token{let};
        if (s == sqrtKey) return Token{squareRoot};
        if (s == powerKey) return Token{power};
        if (s == constKey) return Token{declareConst};
        return Token{name, s};
    }
    throw std::runtime_error("Bad token");
}

double handleNameTokens(Token& t) {
        double result = varTable.getValue(t.name);
        std::string varName = t.name;
        t = ts.get();

        if (t.type == '=')
            result = varTable.reassign(varName);
        else
            ts.putback(t);
        return result;
}

double handleBrackets(Token& t) {
    double result;
    switch (t.type) {
        case '(': {
            result = second_order();
            t = ts.get();
            if (t.type != ')')
                throw std::runtime_error("Error, missing ')'");
            return result;
        }
        case '{': {
            result = second_order();
            t = ts.get();
            if (t.type != '}')
                throw std::runtime_error("Error, expected '}'.");
            return result;
        }
        default:
            throw std::runtime_error("'(' or '{' expected.");
    }
}

double handleDivision(Token& t, double lhs) {
    double divisor = factorial();
    if (divisor == 0)
        throw std::runtime_error("Error, division by 0!");
    return lhs / divisor;
}

double handleModulo(Token& t, double lhs) {
    int rhs;
    rhs = static_cast<int>(factorial());
    if (rhs == 0) 
        throw std::runtime_error("Error, division by 0!");
    return static_cast<int>(lhs) % rhs;
}

double handleSqrt() {
    Token t = ts.get();
    if (t.type != '(') throw std::runtime_error("Expected '('");

    double result = second_order();
    if (result < 0) throw std::runtime_error("Sqrt is only available for n > 0");

    Token t2 = ts.get();

    if (t2.type != ')') throw std::runtime_error("Expected ')'");
    return sqrt(result);
}

double handlePow() {
    Token t = ts.get();
    if (t.type != '(') throw std::runtime_error("Expected '('");

    double base = second_order();
    Token t2 = ts.get();
    if (t2.type != ',') throw std::runtime_error("Expected ','");

    double exponent = static_cast<int>(second_order());
    double result = pow(base, exponent);

    Token t3 = ts.get();
    if (t3.type != ')') throw std::runtime_error("Expected ')'");
    return result;
}



void cleanUp() {
    ts.ignore(print);
}

void calculate() {
    double result;
    while (std::cin)
    {
        Token t = ts.get();
        try {
            if (t.type == quit) break;
            if (t.type == print) {
                std::cout << result << std::endl << prompt;
                continue;
            }
            else if (t.type == help) {
                printHelp();
                std::cout << prompt;
            }
            else ts.putback(t);
            result = statement();

        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n' << prompt;
            cleanUp();
        }
    }
}

void printHelp() {
    std::cout << "Welcome to the calculator.\n\tHOW TO:\n";
    std::cout << "To calculate the result of your input end it with ';' symbol and press Return (Enter).\n";
    std::cout << "The calculator provides all basic operations: -, +, /, * and %.\n";
    std::cout << "The order of operations is PEMDAS.\n";
    std::cout << "There is also a factorial function, used with x!, its precedence is higher than multi/division.\n";
    std::cout << "Parentheses (both () and {}) are available to control the order.\n";
    std::cout << "Functions sqrt(n) and pow(m,i), where n is > 0 and i is an integer are available.\n";
    std::cout << "User defined variables can be declared with 'let variable_name = value'.\n";
    std::cout << "User defined variable values can be reassigned using 'variable_name = new value'.\n";
    std::cout << "User defined variables can be used in calculation by simply using 'variable_name' instead of a number.\n";
}
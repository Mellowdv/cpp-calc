#include "parser.h"
#include "tokens.h"
#include "variables.h"
#include <iostream>
#include <cmath>

int calculateFactorial(int n);

double statement(TokenStream& ts) {
    Token t = ts.get();
    switch (t.type) {
    case let:
        return declaration(ts);
    case declareConst:
        return declaration(ts, newConst);
    case squareRoot: {
        return handleSqrt(ts);
    }
    case power: {
        return handlePow(ts);
    }
    default:
        ts.putback(t);
        return second_order(ts);
    }
}

double primary(TokenStream& ts)
{
    double result;
    std::string varName;
    Token t = ts.get();

    switch (t.type) {
    case number: 
        return t.value;
    case name:
        return handleNameTokens(ts, t);
    case '+':
        return primary(ts);
    case '-':
        return -primary(ts);
    case '(':
        return handleBrackets(ts, t);
    case '{':
        return handleBrackets(ts, t);
    case squareRoot:
        return handleSqrt(ts);
    case power:
        return handlePow(ts);
    default:
        throw std::runtime_error("Primary expected.");
    }
}

double factorial(TokenStream& ts) {

    double lhs = primary(ts);
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

double second_order(TokenStream& ts)
{
    double lhs = first_order(ts);
    Token t = ts.get();

    while (true) {
        if (t.type == '+') {
            lhs += first_order(ts);
            t = ts.get();
        }
        else if (t.type == '-') {
            lhs -= first_order(ts);
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

double first_order(TokenStream& ts)
{
    double lhs = factorial(ts);
    Token t = ts.get();

    while (true) {
        if (t.type == '*') {
            lhs *= factorial(ts);
            t = ts.get();
        }
        else if (t.type == '/') {
            lhs = handleDivision(ts, t, lhs);
            t = ts.get();
        }
        else if (t.type == '%') {
            lhs = handleModulo(ts, t, lhs);
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

double declaration(TokenStream& ts, bool isConst) {
    Token t = ts.get();
    if (t.type != name) throw std::runtime_error("Expected a name.");
    std::string varName = t.name;

    Token t2 = ts.get();
    if (t2.type != '=') throw std::runtime_error("Expected assignement.");
    
    double d = second_order(ts);
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

double handleNameTokens(TokenStream& ts, Token& t) {
        double result = varTable.getValue(t.name);
        std::string varName = t.name;
        t = ts.get();

        if (t.type == '=')
            result = varTable.reassign(varName);
        else
            ts.putback(t);
        return result;
}

double handleBrackets(TokenStream& ts, Token& t) {
    double result;
    switch (t.type) {
        case '(': {
            result = second_order(ts);
            t = ts.get();
            if (t.type != ')')
                throw std::runtime_error("Error, missing ')'");
            return result;
        }
        case '{': {
            result = second_order(ts);
            t = ts.get();
            if (t.type != '}')
                throw std::runtime_error("Error, expected '}'.");
            return result;
        }
        default:
            throw std::runtime_error("'(' or '{' expected.");
    }
}

double handleDivision(TokenStream& ts, Token& t, double lhs) {
    double divisor = factorial(ts);
    if (divisor == 0)
        throw std::runtime_error("Error, division by 0!");
    return lhs / divisor;
}

double handleModulo(TokenStream& ts, Token& t, double lhs) {
    int rhs;
    rhs = static_cast<int>(factorial(ts));
    if (rhs == 0) 
        throw std::runtime_error("Error, division by 0!");
    return static_cast<int>(lhs) % rhs;
}

double handleSqrt(TokenStream& ts) {
    Token t = ts.get();
    if (t.type != '(') throw std::runtime_error("Expected '('");

    double result = second_order(ts);
    if (result < 0) throw std::runtime_error("Sqrt is only available for n > 0");

    Token t2 = ts.get();

    if (t2.type != ')') throw std::runtime_error("Expected ')'");
    return sqrt(result);
}

double handlePow(TokenStream& ts) {
    Token t = ts.get();
    if (t.type != '(') throw std::runtime_error("Expected '('");

    double base = second_order(ts);
    Token t2 = ts.get();
    if (t2.type != ',') throw std::runtime_error("Expected ','");

    double exponent = static_cast<int>(second_order(ts));
    double result = pow(base, exponent);

    Token t3 = ts.get();
    if (t3.type != ')') throw std::runtime_error("Expected ')'");
    return result;
}



void cleanUp() {
    ts.ignore(print);
}

void calculate(TokenStream& ts) {
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
            result = statement(ts);

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
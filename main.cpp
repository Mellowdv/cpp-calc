/*

    Calculator

    Based on the contents of the book Programming: Principles and Practice Using C++.

    This calculator implements a simple parser for basic mathematical expressions.
    The input comes through std::cin into Token_stream ts, output through std::cout.

    Grammar for Input:

    Statement:
        Second Order (+/-)
        Print
        Quit

    Print:
        =
    
    Quit:
        q

    Second Order:
        First Order
        Second Order '+' First Order
        Second Order '-' First Order
    
    First Order:
        Factorial
        First Order '*' Factorial
        First Order '/' Factorial
        First Order '%' Factorial
    
    Factorial:
        Primary
        Primary '!'
    
    Primary:
        Number
        ( Second Order )
        { Second Order }
        '-' Primary
        '+' Primary
    
    Number:
        Floating point numerals

*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

double primary();
double first_order();
double second_order();
double handleSqrt();
double handlePow();

const char number = '8';
const char print = ';';
const char quit = 'q';
const std::string prompt = "> ";
const char name = 'a';
const char let = 'L';
const std::string declKey = "let"; 
const char squareRoot = 'S';
const std::string sqrtKey = "sqrt";
const char power = 'P';
const std::string powerKey = "pow";

class Token
{
public:
    char type;
    double value;
    std::string name;
    Token(char new_type, std::string new_name) : type(new_type), name(new_name) {}
    Token(char new_type, double new_value) : type(new_type), value(new_value) {}
    Token(char new_type) : type(new_type), value(0) {}
    Token() {}
};

class TokenStream
{
private:
    Token buffer;
    bool full {false};
public:
    Token get();
    void putback(Token t);
    void ignore(char c);
};

TokenStream ts;

void  TokenStream::ignore(char c) {
    
    // Checking the buffer
    if (full && c == buffer.type) {
        full = false;
        return;
    }
    full = false;

    // Searching input
    char ch = 0;
    while(std::cin >> ch) {
        if (ch == c) return;
    }
}

void TokenStream::putback(Token t) {
    if (full) throw std::runtime_error("Putback into a full buffer.");
    buffer = t;
    full = true;
}

Token TokenStream::get()
{

    if (full) {
        full = false;
        return buffer;
    }

    char input;
    double val;
    std::cin >> input;

    switch (input)
    {
        case '.':
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            std::cin.putback(input);
            std::cin >> val;
            return Token(number, val);
        }
        case '+': case '-': 
        case '*': case '/': 
        case '(': case ')': 
        case '%': case '=':
        case '{': case '}': 
        case '!': case ',':
            return Token(input);
        case print:
            return Token(print);
        case quit:
            return Token(quit);
        default:
        {
            if (isalpha(input)) {
                std::string s;
                s += input;
                while (std::cin.get(input) && (isalpha(input) || isdigit(input))) s += input;
                std::cin.putback(input);
                if (s == declKey) return Token{let};
                if (s == sqrtKey) return Token{squareRoot};
                if (s == powerKey) return Token{power};
                return Token{name, s};
            }
            throw std::runtime_error("Bad token");
        }
    }
}

class Variable {
public:
    std::string name;
    double value;
    bool isConst;

    Variable(std::string newName, double newValue) : name(newName), value(newValue) {}
};

std::vector<Variable> varTable;

double getValue(std::string s) {
    for(const Variable& var: varTable) {
        if (var.name == s) return var.value;
    }
    throw std::runtime_error("Variable not found.");
}

void setValue(std::string s, double d) {
    for(Variable& var: varTable) {
        if (var.name == s) {
            var.value = d;
            return;
        }
    }
    throw std::runtime_error("Variable not found.");
}

bool isDeclared(std::string s) {
    for (Variable var: varTable) {
        if (var.name == s) 
            return true;
    }
    return false;
}

double defineName(std::string s, double d) {
    if (isDeclared(s)) throw std::runtime_error("Declared twice.");
    varTable.push_back(Variable(s, d));
    return d;
}

double declaration() {
    Token t = ts.get();
    if (t.type != name) throw std::runtime_error("Expected a name.");
    std::string varName = t.name;

    Token t2 = ts.get();
    if (t2.type != '=') throw std::runtime_error("Expected assignement.");
    
    double d = second_order();
    defineName(varName, d);
    return d;
}

int calculateFactorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * calculateFactorial(n - 1);
}


double primary()
{
    double result;
    Token t = ts.get();
    if (t.type == number) {
        result = t.value;
        return result;
    }
    else if (t.type == name) {
        result = getValue(t.name);
        return result;
    }
    else if (t.type == '+')
        return primary();
    else if (t.type == '-')
        return -primary();
    else if (t.type == '(') {
        result = second_order();
        t = ts.get();
        if (t.type != ')')
            throw std::runtime_error("Error, missing ')'");
        return result;
    }
    else if (t.type == '{') {
        result = second_order();
        t = ts.get();
        if (t.type != '}')
            throw std::runtime_error("Error, expected '}'.");
        return result;
    }
    else if (t.type == squareRoot)
        return handleSqrt();
    else if (t.type == power)
        return handlePow();
    else
        throw std::runtime_error("Primary expected.");
}


double factorial() {

    double lhs = primary();
    Token t = ts.get();
    if (t.type == '!')
    {
        lhs = calculateFactorial(lhs);
        return lhs;
    }
    else
    {
        ts.putback(t);
        return lhs;
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
            double divisor = factorial();
            if (divisor == 0) {
                throw std::runtime_error("Error, division by 0!");
            }
            else {
                lhs /= divisor;
                t = ts.get();
            }
        }
        else if (t.type == '%') {
            int rhs;
            rhs = static_cast<int>(factorial());
            if (rhs == 0) {
                throw std::runtime_error("Error, division by 0!");
            }
            lhs = static_cast<int>(lhs) % rhs;
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
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

double statement() {
    Token t = ts.get();
    switch (t.type) {
    case let:
        return declaration();
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
            else ts.putback(t);
            result = statement();

        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            cleanUp();
        }
    }
}

int main()
{
    try {
        defineName("pi", 3.1415926535);
        defineName("e", 2.7182818284);
        defineName("k", 1000);
        std::cout << prompt;
        calculate();
        return 0;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Exception" << std::endl;
        return 2; 
    }
}
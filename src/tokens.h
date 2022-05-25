#ifndef TOKENS_H
#define TOKENS_H
#include <string>

const char number = '8';
const char print = ';';
const char quit = 'q';
const char name = 'a';
const char let = 'L';
const char help = 'h';
const char HELP = 'H';
const std::string declKey = "let"; 
const char squareRoot = 'S';
const std::string sqrtKey = "sqrt";
const char power = 'P';
const std::string powerKey = "pow";
const char declareConst = 'C';
const std::string constKey = "const";
const char fromFile = 'f';
const std::string fromKey = "from";
const char toFile = 't';
const std::string toKey = "to";

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

// Global variable used for all tokenstream operations.
extern TokenStream ts;

// Global variables for reading and writing from files
extern bool isReading;
extern bool isWriting;

#endif
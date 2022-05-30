#ifndef PARSER_H
#define PARSER_H
#include "tokens.h"

const bool newConst = true;
const std::string prompt = "> ";

// trivial (i.e. 1-2 lines of code) mathematical operations and variable declaration
double statement(TokenStream& ts);
double primary(TokenStream& ts);
double factorial(TokenStream& ts);
double second_order(TokenStream& ts);
double first_order(TokenStream& ts);
double declaration(TokenStream& ts, bool isConst = false);

// handling of various non-trivial maths operations
Token handleStringInput(char& input);
double handleNameTokens(TokenStream& ts, Token& t);
double handleBrackets(TokenStream& ts, Token& t);
double handleDivision(TokenStream& ts, Token& t, double lhs);
double handleModulo(TokenStream& ts, Token& t, double lhs);
double handleSqrt(TokenStream& ts);
double handlePow(TokenStream& ts);

// file reading and writing
double handleFileRead(TokenStream& ts);

// main calculation loop, error clean up and help
void calculate(TokenStream& ts);
void cleanUp();
void printHelp();

extern std::ifstream instructions;

#endif
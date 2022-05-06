#ifndef PARSER_H
#define PARSER_H
#include "tokens.h"

const bool newConst = true;
const std::string prompt = "> ";

// trivial (i.e. 1-2 lines of code) mathematical operations and variable declaration
double statement();
double primary();
double factorial();
double second_order();
double first_order();
double declaration(bool isConst = false);

// handling of various non-trivial maths operations
Token handleStringInput(char& input);
double handleNameTokens(Token& t);
double handleBrackets(Token& t);
double handleDivision(Token& t, double lhs);
double handleModulo(Token& t, double lhs);
double handleSqrt();
double handlePow();

// main calculation loop, error clean up and help
void calculate();
void cleanUp();
void printHelp();

#endif
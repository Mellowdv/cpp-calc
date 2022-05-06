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
#include "parser.h"
#include "variables.h"
#include "tokens.h"

TokenStream ts;
SymbolTable varTable;

// helper function for calculating factorials
int calculateFactorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * calculateFactorial(n - 1);
}

int main()
{
    try {
        varTable.defineName("pi", 3.1415926535);
        varTable.defineName("e", 2.7182818284);
        varTable.defineName("k", 1000);
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
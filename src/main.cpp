/*

    Calculator

    Based on the contents of the book Programming: Principles and Practice Using C++.

    This calculator implements a simple parser for basic mathematical expressions.
    The input comes through std::cin into Token_stream ts, output through std::cout.

    Grammar for Input:

    Statement:
        Second Order'Print'
        Second Order
        Declaration
        Reassignment
        Power
        Square Root
        Print
        Quit
        Help

    Declaration:
        'let' name '=' Second Order'Print'
        'const' name '=' Second Order'Print'

    Reassignment:
        name '=' Second Order'Print'

    Power:
        pow(Primary, Primary)
        pow(Primary, Primary)'Print'

    Square Root:
        sqrt(Primary)
        sqrt(Primary)'Print'

    Print:
        ;
    
    Quit:
        q

    Help:
        h or H

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
        Primary'!'
    
    Primary:
        Variable
        Number
        ( Second Order )
        { Second Order }
        '-' Primary
        '+' Primary
    
    Variable:
        Name
        
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
bool isReading;
bool isWriting;

// helper function for calculating factorials
int calculateFactorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * calculateFactorial(n - 1);
}

int main()
{
    std::cout << "For help please input 'h' or 'H' followed by Enter.\n";
    std::cout << "To quit please input 'q' followed by Enter.\n";
    try {
        varTable.defineName("pi", 3.1415926535);
        varTable.defineName("e", 2.7182818284);
        varTable.defineName("k", 1000);
        std::cout << prompt;
        calculate(ts);
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
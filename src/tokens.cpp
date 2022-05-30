#include "tokens.h"
#include "parser.h"
#include <iostream>
#include <fstream>

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
    if (instructions.is_open() && instructions.eof()) {
        try {
            instructions.close();
            isReading = false;
            std::cout << "File read completed." << std::endl;
        }
        catch(std::exception& e) {
            std::cerr << "Error: " << e.what();
        }
    }
    if (full) {
        full = false;
        return buffer;
    }
    char input;
    double val;

    if (isReading) {
        instructions >> input;        
    }
    else
        std::cin >> input;

    switch (input)
    {
        case '.':
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            if (isReading) {
                instructions.putback(input);
                instructions >> val;    
            }
            else {
                std::cin.putback(input);
                std::cin >> val;
            }
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
        case help: case HELP:
            return Token(help);
        case '\0':
            return Token(input);
        default:
            return handleStringInput(input);
    }
}
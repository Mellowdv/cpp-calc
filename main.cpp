#include <iostream>
#include <string>

const char number = '8';

class Token
{
public:
    char type;
    int value;
    Token(char new_type, int new_value) : type(new_type), value(new_value) {}
    Token(char new_type) : type(new_type), value(0) {}
    Token() {}

    void display_token()
    {
        if (type == number)
            std::cout << value;
        else
            std::cout << type;
    }
};

class TokenStream
{
public:
    Token symbol;
};

TokenStream ts;

Token create_token(char input)
{
    Token symbol;
    switch (input)
    {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            symbol = Token(number, (int(input)-48));
            break;
        }
        case '+': case '-': case '*': case '/': case '(': case ')':
        {
            symbol = Token(input);
            break;
        }
        default:
            break;
    } 
    return symbol;
}

double evaluate();

double primary()
{
    char input;
    std::cin >> input;
    double value;
    Token tok;

    value = ts.symbol.value;

    if (isdigit(input))
    {
        ts.symbol.value = value*10 + (int(input) - 48);
        primary();
    }
    else
    {
        value = ts.symbol.value;
        ts.symbol = Token(input);
        return value;
    }
}

double evaluate()
{
    char input;
    double lhs;

    Token tok;

    if (ts.symbol.type == number)
        lhs = primary();
    else if (ts.symbol.type == ';')
        return lhs;
    if (ts.symbol.type == '+')
    {
        std::cin >> input;
        ts.symbol = create_token(input);
        lhs = lhs + primary();
        return lhs + evaluate();
    }
    return lhs;
}

int main()
{
    char input;
    Token empty(' ');
    Token tok;
    double result;

    while (std::cin >> input)
    {
        tok = create_token(input);
        ts.symbol = tok;
        if (input == 'q')
            break;
        std::cout << evaluate();
    }
    return 0;
}
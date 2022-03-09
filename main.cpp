#include <iostream>
#include <string>

double primary();
double first_order();
double second_order();

const char number = '8';
const char print = '=';

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

Token create_token()
{
    char input;
    std::cin >> input;
    switch (input)
    {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            return Token(number, (int(input)-48));
            break;
        }
        case '+': case '-': case '*': case '/': case '(': case ')':
        {
            return Token(input, ts.symbol.value);
            break;
        }
        case '=':
            return Token(print, ts.symbol.value);
        case 'q':
        {
            return Token(input);
        }
        default:
            break;
    }
    return Token(input);
}

void char_to_num()
{
    double value;

    value = ts.symbol.value;
    ts.symbol = create_token();

    if (ts.symbol.type == number)
    {
        ts.symbol.value = value*10 + (ts.symbol.value);
        char_to_num();
    }
    return;
}

double primary()
{
    double result;
    if (ts.symbol.type == print)
        return ts.symbol.value;

    char_to_num();
    result = first_order();
    return result;
}

// this function evaluates the first order ops, * and /
double first_order()
{
    double result;
    double lhs = ts.symbol.value;

    if (ts.symbol.type == '*')
    {
        ts.symbol = create_token();
        char_to_num();
        result = lhs * ts.symbol.value;
        return result;
    }
    else if (ts.symbol.type == '/')
    {
        ts.symbol = create_token();
        char_to_num();
        return lhs / ts.symbol.value;
    }
    return second_order();
}

double second_order()
{
    double lhs = ts.symbol.value;
    double result;

    if (ts.symbol.type == '+')
    {
        ts.symbol = create_token();
        char_to_num();
        result = lhs + first_order();
        return result;
    }
    if (ts.symbol.type == '-')
    {
        ts.symbol = create_token();
        char_to_num();
        return lhs - first_order();
    }
    return lhs;
}

int main()
{
    char input;
    double result;

    while (ts.symbol.type != 'q')
    {
        ts.symbol = create_token();
        primary();
        std::cout << first_order() << std::endl;
    }
    return 0;
}
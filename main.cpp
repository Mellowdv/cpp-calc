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
    double value;
    Token(char new_type, double new_value) : type(new_type), value(new_value) {}
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
    double val;
    std::cin >> input;

    switch (input)
    {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
        {
            std::cin.putback(input);
            std::cin >> val;
            return Token(number, val);
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

double primary()
{
    double result;
    ts.symbol = create_token();
    if (ts.symbol.type == number)
    {
        result = ts.symbol.value;
        ts.symbol = create_token();
        return result;
    }
    else if (ts.symbol.type == '(')
    {
        result = second_order();
        if (ts.symbol.type != ')')
        {
            std::cout << "Error, missing ')'" << std::endl;
            return -1;
        }
        ts.symbol = create_token();
        return result;
    }
    else if (ts.symbol.type == '-')
        return -primary();
    return ts.symbol.value;
}

double first_order()
{
    double result;
    double lhs = primary();

    if (ts.symbol.type == '*')
    {
        result = lhs * first_order();
        return result;
    }
    else if (ts.symbol.type == '/')
    {
        result = lhs / first_order();
        return result;
    }
    return lhs;
}

double second_order()
{
    double lhs = first_order();
    double result;

    if (ts.symbol.type == '+')
    {
        result = lhs + second_order();
        return result;
    }
    if (ts.symbol.type == '-')
    {
        result = lhs - second_order();
        return result;
    }
    return lhs;
}

int main()
{
    char input;
    double result;

    while (true)
    {
        result = second_order();
        if (ts.symbol.type == 'q')
            break;
        if (ts.symbol.type == '=')
            std::cout << result << std::endl;
    }
    return 0;
}
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

    if (ts.symbol.type == '=')
        return;

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
    char_to_num();
    return ts.symbol.value;
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
        ts.symbol.value = result;
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
        if (ts.symbol.type == '+' || ts.symbol.type == '-')
        {
            ts.symbol.value = lhs + ts.symbol.value;
            return ts.symbol.value;
        }
        result = lhs + first_order();
        ts.symbol.value = result;
        return result;
    }
    if (ts.symbol.type == '-')
    {
        ts.symbol = create_token();
        char_to_num();
        if (ts.symbol.type == '+' || ts.symbol.type == '-')
        {
            ts.symbol.value = lhs - ts.symbol.value;
            return ts.symbol.value;
        }
        ts.symbol.value = lhs - first_order();
        return ts.symbol.value;
    }
    return primary();
}

double evaluate()
{
    double result;
    while (ts.symbol.type != '=' && ts.symbol.type != 'q')
    {
        result = first_order();
    }
    return result;
}

int main()
{
    char input;
    double result;

    while (true)
    {
        ts.symbol = create_token();
        result = evaluate();
        if (ts.symbol.type == 'q')
            break;
        std::cout << result << std::endl;
    }
    return 0;
}
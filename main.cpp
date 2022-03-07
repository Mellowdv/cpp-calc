#include <iostream>
#include <string>

const char number = '8';


class Token
{
private:
    char type;
    int value;
public:
    Token(char new_type, int new_value) : type(new_type), value(new_value) {}
    Token(char new_type) : type(new_type), value(0) {}
    Token() {}

    char get_type()
    {
        return type;
    }

    int get_value()
    {
        return value;
    }

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
private:
    Token symbol;
public:
    void set_symbol(Token new_symbol)
    {
        symbol = new_symbol;
    }

    Token get_symbol()
    {
        return symbol;
    }
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

double evaluate()
{
    Token current_token = ts.get_symbol();
    Token next_token;
    char input;
    if (current_token.get_type() == number)
    {
        std::cin >> input;
        next_token = create_token(input);
        ts.set_symbol(next_token);
        evaluate();
        return 0;
    }
    else
    {
        std::cout << "\nI found a non-number!";
        return 0;
    }
}

double add_subtract()
{
    
}

int main()
{
    char input;
    Token test;

    while (std::cin >> input)
    {
        if (input == 'q')
            break;
        test = create_token(input);
        ts.set_symbol(test);
        evaluate();
    }
    return 0;
}
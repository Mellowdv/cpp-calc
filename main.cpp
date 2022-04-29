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
};

class TokenStream
{
private:
    Token buffer;
    bool full {false};
public:
    Token get();
    void putback(Token t);
};

void TokenStream::putback(Token t) {
    if (full) throw std::runtime_error("Putback into a full buffer.");
    buffer = t;
    full = true;
}

Token TokenStream::get()
{

    if (full) {
        full = false;
        return buffer;
    }

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
        case '+': case '-': case '*': case '/': case '(': case ')': case '%':
        {
            return Token(input);
            break;
        }
        case '=':
            return Token(print);
        case 'q':
        {
            return Token(input);
        }
        default:
            throw std::runtime_error("Bad token.");
    }
}

TokenStream ts;

double primary()
{
    double result;
    Token t = ts.get();
    if (t.type == number)
    {
        result = t.value;
        return result;
    }
    else if (t.type == '+')
        return primary();
    else if (t.type == '-')
        return -primary();
    else if (t.type == '(')
    {
        result = second_order();
        t = ts.get();
        if (t.type != ')')
        {
            throw std::runtime_error("Error, missing ')'");
        }
        return result;
    }
    else
        throw std::runtime_error("Primary expected.");
}

double first_order()
{
    double lhs = primary();

    Token t = ts.get();

    while (true) {
        if (t.type == '*')
        {
            lhs *= primary();
            t = ts.get();
        }
        else if (t.type == '/')
        {
            double divisor = primary();
            if (divisor == 0)
            {
                throw std::runtime_error("Error, division by 0!");
            }
            else
            {
                lhs /= divisor;
                t = ts.get();
            }
        }
        else if (t.type == '%')
        {
            int rhs;
            rhs = static_cast<int>(primary());
            if (rhs == 0)
            {
                throw std::runtime_error("Error, division by 0!");
            }
            lhs = static_cast<int>(lhs) % rhs;
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

double second_order()
{
    double lhs = first_order();
    Token t = ts.get();

    while (true) {
        if (t.type == '+')
        {
            lhs += first_order();
            t = ts.get();
        }
        else if (t.type == '-')
        {
            lhs -= first_order();
            t = ts.get();
        }
        else {
            ts.putback(t);
            return lhs;
        }
    }
}

int main()
{
    double result;
    std::cout << "> ";

    while (std::cin)
    {
        Token t = ts.get();
        try
        {
            if (t.type == 'q') break;
            if (t.type == '=') { 
                std::cout << result << std::endl << "> ";
                continue;
            }
            else ts.putback(t);
            result = second_order();

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return 0;
}
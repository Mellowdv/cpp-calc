#ifndef VARIABLES_H
#define VARIABLES_H
#include <string>
#include <vector>

class Variable {
public:
    std::string name;
    double value;
    bool isConst {false};

    Variable(std::string newName, double newValue) : name(newName), value(newValue) {}
    Variable(std::string newName, double newValue, bool newIsConst) : name{newName}, value{newValue}, isConst{newIsConst} {}
};

class SymbolTable {
private:
    std::vector<Variable> varTable;

public:
    double getValue(std::string s);
    void setValue(std::string s, double val);
    bool isDeclared(std::string s);
    double defineName(std::string s, double d, bool isConst = false);
    double reassign(std::string varName);
};

// Global variable used for all variable operations in the parser.
extern SymbolTable varTable;

#endif
#include "variables.h"
#include "tokens.h"
#include <iostream>
#include <string>

double SymbolTable::getValue(std::string s) {
    for(const Variable& var: varTable) {
        if (var.name == s) return var.value;
    }
    throw std::runtime_error("Variable not found.");
}

void SymbolTable::setValue(std::string s, double d) {
    for(Variable& var: varTable) {
        if (var.name == s) {
            if (var.isConst)
                throw std::runtime_error("Unable to change a const's value");
            else {
                var.value = d;
                return;
            }
        }
    }
    throw std::runtime_error("Variable not found.");
}

bool SymbolTable::isDeclared(std::string s) {
    for (Variable var: varTable) {
        if (var.name == s) 
            return true;
    }
    return false;
}

double SymbolTable::defineName(std::string s, double d, bool isConst) {
    if (isDeclared(s)) throw std::runtime_error("Declared twice.");
    if (isConst)
        varTable.push_back(Variable(s, d, isConst));
    else
        varTable.push_back(Variable(s, d));
    return d;
}

double SymbolTable::reassign(std::string varName) {
    Token t = ts.get();
    setValue(varName, t.value);
    return t.value;
}
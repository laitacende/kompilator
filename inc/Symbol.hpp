#ifndef KOMPILATOR_SYMBOL_HPP
#define KOMPILATOR_SYMBOL_HPP

#include <string>

#include "Variable.hpp"

class Symbol {
public:
    Symbol(std::string name, long long int mem, Variable* var);
    bool isVariable = false;
    bool isConstant = false;
    bool isInit = false;
    std::string name;
    long long int memoryAddress;
    Variable* var;
};
#endif //KOMPILATOR_SYMBOL_HPP

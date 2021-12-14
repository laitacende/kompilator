#ifndef KOMPILATOR_SYMBOL_HPP
#define KOMPILATOR_SYMBOL_HPP

#include <string>

class Symbol {
public:
    Symbol(std::string name, long long int mem);
    bool isVariable = false;
    bool isConstant = false;
    bool isInit = false;
    std::string name;
    long long int memoryAddress;
};
#endif //KOMPILATOR_SYMBOL_HPP

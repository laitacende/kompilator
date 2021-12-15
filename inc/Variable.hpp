#ifndef KOMPILATOR_VARIABLE_HPP
#define KOMPILATOR_VARIABLE_HPP

#include <string>

class Variable {
public:
    Variable(std::string name, long long int address);
    std::string name;
    long long int address;
    bool isVariable = false;
    bool isConstant = false;
    bool isInit = false;
    long long int val;
};
#endif //KOMPILATOR_VARIABLE_HPP
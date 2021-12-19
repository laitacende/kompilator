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
    bool isArray = false;
    long long int size = 1;
    long long int startArray = 0;
    long long int endArray = 0;

};
#endif //KOMPILATOR_VARIABLE_HPP

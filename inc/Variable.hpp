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
    long long int offset = 0; // address of variable when accessing array with variable
    bool isArrayWithVar = false; // true when accessing array with variable
    bool isIterator = false;
    long long int fromIterator = 0; // address of variable from in loop
    long long int toIterator = 0; // address of variable in end condition in for loop

};
#endif //KOMPILATOR_VARIABLE_HPP

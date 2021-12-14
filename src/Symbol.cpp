#include "../inc/Symbol.hpp"

Symbol::Symbol(std::string name, long long int mem) {
    this->name = name;
    isVariable = false;
    isConstant = false;
    isInit = false;
    memoryAddress = mem;
}

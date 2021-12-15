#include "../inc/Variable.hpp"

Variable::Variable(std::string name, long long int address) {
    this->name = name;
    this->address = address;
}
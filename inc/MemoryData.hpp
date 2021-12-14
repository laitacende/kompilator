#ifndef KOMPILATOR_MEMORYDATA_HPP
#define KOMPILATOR_MEMORYDATA_HPP

#include <unordered_map>
#include <memory>

#include "../inc/Symbol.hpp"

// operations on memory, variable table...
class MemoryData {
public:
    MemoryData();
    long long int addVariable(std::string name);
    long long int addConstant(long long int val);
    bool ifExists(std::string);
    //int initVariable(std::string name, long long int val);
    long long int currentAddress = 0;
    long long int getAddress(std::string name);
private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> memo;
};
#endif //KOMPILATOR_MEMORYDATA_HPP

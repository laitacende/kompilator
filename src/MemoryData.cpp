#include "../inc/MemoryData.hpp"
#include "../inc/Symbol.hpp"

#include <unordered_map>
#include <memory>
#include <iostream>

MemoryData::MemoryData() {
    currentAddress = 0;
}

bool MemoryData::ifExists(std::string name) {
    std::unordered_map<std::string, std::shared_ptr<Symbol>>::iterator it = memo.find(name);

    if(it != memo.end()) {
        return true;
    } else {
        return false;
    }
}

long long int MemoryData::addVariable(std::string name) {
    // check if variable exists
    if (!ifExists(name)) {
        // allocate memory
        Variable* var = new Variable(name, currentAddress);
        var->isVariable = true;
        var->isInit = false;
        memo[name] = std::make_shared<Symbol>(name, currentAddress, var);
        currentAddress++;
        memo[name]->isVariable = true;
        memo[name]->isInit = false;
        return currentAddress - 1;
    }
    return -1;
}

long long int MemoryData::addConstant(long long int val) {
    std::string name = std::to_string(val);
    //if (!ifExists(name)) {
        // allocate memory
        Variable* var = new Variable(name, currentAddress);
        var->isConstant = true;
        var->val = val;
        memo[name] = std::make_shared<Symbol>(name, currentAddress, var);
        currentAddress++;
        memo[name]->isVariable = false;
        memo[name]->isConstant = true;
        return currentAddress - 1;
   // }

   // return -1;
}

long long int MemoryData::addVariableOfSize(std::string name, long long int size, long long int start, long long int end) {
    if (!ifExists(name)) {
        // allocate memory
        Variable* var = new Variable(name, currentAddress);
        var->isInit = false;
        var->size = size;
        var->isArray = true;
        var->startArray = start;
        var->endArray = end;
        memo[name] = std::make_shared<Symbol>(name, currentAddress, var);
        currentAddress += size;
        currentAddress++;
        return currentAddress - 1 - size;
    }
    return -1;
}

//int MemoryData::initVariable(std::string name, long long int val) {
//    // check if variable exists in memory
//    if (memo.find(name) != memo.end) {
//        // change value in memory
//
//        return 0;
//    }
//    return 1;
//}

long long int MemoryData::getAddress(std::string name) {
    if (ifExists(name)) {
        return memo[name]->memoryAddress;
    }
    return -1;
}

Variable* MemoryData::getVar(std::string name) {
    if (ifExists(name)) {
        return memo[name]->var;
    }
    return nullptr;
}


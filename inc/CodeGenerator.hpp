#ifndef KOMPILATOR_CODEGENERATOR_HPP
#define KOMPILATOR_CODEGENERATOR_HPP

#include <vector>
#include <string>
#include <memory>

#include "MemoryData.hpp"

class CodeGenerator {
public:
    CodeGenerator(std::shared_ptr<MemoryData> controller);
    std::vector<std::string> getCode();
    void addInstruction(std::string newInstr);
    // TODO tu trzeba jakis inny typ, bo nie wiadomo czy stala czy zmienna
    void write(long long int val);
    long long int makeConstant(long long int val);
    long long int getAddress(std::string name);
    bool getConstant(std::string name);
    long long int allocateConstant(long long int value);
    bool allocateVariable(std::string name);
    bool assignToVariable(long long int addr, long long int val);
private:
    std::vector<std::string> code;
    int offset;
    std::shared_ptr<MemoryData> memo;
};
#endif //KOMPILATOR_CODEGENERATOR_HPP

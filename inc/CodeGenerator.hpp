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
    int addInstruction(std::string newInstr);
    // TODO tu trzeba jakis inny typ, bo nie wiadomo czy stala czy zmienna
    bool write(Variable* var);
    long long int makeConstant(long long int val);
    long long int getAddress(std::string name);
    bool getConstant(std::string name);
    Variable* allocateConstant(long long int value);
    Variable* allocateVariable(std::string name);
    bool assignToVariable(Variable* var1, Variable* var2);
    Variable* getVar(std::string name);
    bool subtract(Variable* var1, Variable* var2);
    bool add(Variable* var1, Variable* var2);
    bool multiply(Variable* var1, Variable* var2);
private:
    std::vector<std::string> code;
    int offset;
    std::shared_ptr<MemoryData> memo;
};
#endif //KOMPILATOR_CODEGENERATOR_HPP

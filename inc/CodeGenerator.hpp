#ifndef KOMPILATOR_CODEGENERATOR_HPP
#define KOMPILATOR_CODEGENERATOR_HPP

#include <vector>
#include <string>

class CodeGenerator {
public:
    CodeGenerator();
    std::vector<std::string> getCode();
    void addInstruction(std::string newInstr);
    // TODO tu trzeba jakis inny typ, bo nie wiadomo czy stala czy zmienna
    void write(long long int val);
    void makeConstant(long long int val);
private:
    std::vector<std::string> code;
    int offset;
};
#endif //KOMPILATOR_CODEGENERATOR_HPP

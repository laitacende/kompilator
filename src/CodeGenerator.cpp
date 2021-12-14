#include "../inc/CodeGenerator.hpp"

#include <vector>
#include <string>


CodeGenerator::CodeGenerator() {
    this->offset = 0;
}
std::vector<std::string> CodeGenerator::getCode() {
    return this->code;
}


void CodeGenerator::addInstruction(std::string newInstr) {
    this->code.push_back(newInstr);
    offset++;
}

// ----------------------------------- UTILS -----------------------------------

void CodeGenerator::makeConstant(long long int val) {
    // generate code to make constant (result in register A)

    // clear register A
    addInstruction("RESET a");
    if (val == 0) {
        return;
    }

   // out.push_back("RESET C");

    addInstruction("INC a");
    if (val == 1) {
        return;
    }

    addInstruction("RESET b");
    addInstruction("INC b"); // B == 1
   // out.push_back("DEC C"); // C == -1

    long long int prevVal = val;
    while (val != 1) {
        prevVal = val;
        val = val >> 1; // val /= 2
        // shift A
        addInstruction("SHIFT b");
        if (val << 1 != prevVal) { // val % 2 == 1
            addInstruction("ADD b");
        }
    }
}


// ----------------------------------- OPERATIONS -------------------------------

void CodeGenerator::write(long long int val) {
    makeConstant(val);
    addInstruction("PUT"); // output register A
}

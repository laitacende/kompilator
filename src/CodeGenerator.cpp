#include "../inc/CodeGenerator.hpp"
#include "../inc/MemoryData.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <bitset>
#include <iostream>


CodeGenerator::CodeGenerator(std::shared_ptr<MemoryData> controller) {
    this->offset = 0;
    memo = controller;
}
std::vector<std::string> CodeGenerator::getCode() {
    return this->code;
}


void CodeGenerator::addInstruction(std::string newInstr) {
    this->code.push_back(newInstr);
    offset++;
}

// ----------------------------------- UTILS -----------------------------------

long long int CodeGenerator::makeConstant(long long int val) {
    // generate code to make constant (result in register A)
    // TODO make negation or sth for negative
    // clear register A
    addInstruction("RESET a");
    if (val == 0) {
        return 0;
    }
    bool isNeg = false;
    if (val < 0) {
        val = -val;
        isNeg = true;
    }
    addInstruction("INC a");
    if (val == 1) {
        return 1;
    }

    addInstruction("RESET b");
    addInstruction("INC b"); // B == 1

    // find number of bits in val
    int counter = 0;
    long long int tmp = val;
    while (tmp >= 1) {
        tmp = tmp >> 1;
        counter++;
    }

    // get binary string
    std::string binary = std::bitset<sizeof(long long int)>(val).to_string();

    // iterate over string
    for (unsigned int i = binary.length() - counter + 1; i < binary.length(); i++) {
        addInstruction("SHIFT b");
        if (binary[i] == '1') {
            addInstruction("ADD b");
        }
    }

    if(isNeg) {
        // TODO odjac od 0 najlatwiej
    }

    return val;
}

long long int CodeGenerator::allocateConstant(long long int value) {
    long long int address = memo->addConstant(value);
    if (address != -1) {
        makeConstant(address);
        addInstruction("SWAP c");
        // constant was allocated under address
        makeConstant(value); // result in register a
        addInstruction("STORE c");
    } else {
        address = memo->getAddress(std::to_string(value));
    }
    return address; // TODO handle errors, constant probably is somewhere, return
}

bool CodeGenerator::allocateVariable(std::string name) {
    long long int address = memo->addVariable(name);
    return address != -1; // TODO handle errors, constant probably is somewhere
}

bool CodeGenerator::assignToVariable(long long int addr, long long int val) {
    if (addr != -1) {
        // TODO maybe check if const isn't in memo, that could save time (ale tylko dla duzych, trzeba popatrzec dla jakich)
        makeConstant(addr);
        addInstruction("SWAP c");
        makeConstant(val); // in register a
        addInstruction("STORE c");
        return true;
    }
    return false;
}

long long int CodeGenerator::getAddress(std::string name) {
    return memo->getAddress(name);
}

bool CodeGenerator::getConstant(std::string name) {
    if (memo->ifExists(name)) {
        long long int address = memo->getAddress(name);

        return true;
    }
    return false; // TODO handle errors

}

// ----------------------------------- OPERATIONS -------------------------------

void CodeGenerator::write(long long int address) {
    //makeConstant(val);
    // get from memory and load to a
    // TODO jak zmienna to sprawdzic czy zainicjalizowana
    makeConstant(address);
    addInstruction("LOAD a");
    addInstruction("PUT"); // output register A
}

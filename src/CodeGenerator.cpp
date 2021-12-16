#include "../inc/CodeGenerator.hpp"
#include "../inc/MemoryData.hpp"
#include "../inc/Variable.hpp"

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


int CodeGenerator::addInstruction(std::string newInstr) {
    this->code.push_back(newInstr);
    offset++;
    return offset - 1;
}

// ----------------------------------- UTILS -----------------------------------

long long int CodeGenerator::makeConstant(long long int val) {
    // generate code to make constant (result in register A)
    // TODO make negation or sth for negative
    // clear register A
   // addInstruction("( const making " + std::to_string(val) + " )");
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

Variable* CodeGenerator::allocateConstant(long long int value) {
    long long int address = memo->addConstant(value);
    if (address != -1) {
       // addInstruction("( const alloc" + std::to_string(value) + " )");
        makeConstant(address);
        addInstruction("SWAP c");
        // constant was allocated under address
        makeConstant(value); // result in register a
        addInstruction("STORE c");
    }
    return memo->getVar(std::to_string(value));
}

Variable* CodeGenerator::allocateVariable(std::string name) {
    long long int address = memo->addVariable(name);
    if (address != -1) {
        return memo->getVar(name);
    }
    return nullptr; // TODO handle errors, variable probably is somewhere
}

bool CodeGenerator::assignToVariable(Variable* var1, Variable* var2) {
    // TODO var 2 could be variable too -  this is probably handled
    // TODO mozna wczytywac do rejestrow,a nie sa dwa razy do pamieci odwolywac
    if (var1 != nullptr && var2 != nullptr) {
        // TODO maybe check if const isn't in memo, that could save time (ale tylko dla duzych, trzeba popatrzec dla jakich)
        var1->isInit = true;
        var1->val = var2->val;
        // result of expression is in a
        //addInstruction("( assign to " + var1->name + " val " + std::to_string(var2->val) + " )");
        addInstruction("SWAP d"); // result of expression is in d
        makeConstant(var1->address); // in register a
        addInstruction("SWAP c");
        // first opt makeConstant(var2->val); // in register a
        //addInstruction("LOAD " + stdvar2->address);
        addInstruction("SWAP d"); // result of expression
        addInstruction("STORE c");
        return true;
    }
    return false; // variable not defined
}

long long int CodeGenerator::getAddress(std::string name) {
    return memo->getAddress(name);
}

Variable* CodeGenerator::getVar(std::string name) {
    // To wczytywac do jakiegos rejestru, najpierw a, pozniej b itp. i miec taki pseudo stos
    return memo->getVar(name);
}

bool CodeGenerator::getConstant(std::string name) {
    if (memo->ifExists(name)) {
        long long int address = memo->getAddress(name);

        return true;
    }
    return false; // TODO handle errors

}

// ----------------------------------- OPERATIONS -------------------------------

bool CodeGenerator::write(Variable* var) {
    //makeConstant(val);
    // get from memory and load to a
    if (var->isVariable && !var->isInit) {
        return false;
    }
    makeConstant(var->address);
    //addInstruction("( write )");
    addInstruction("LOAD a");
    addInstruction("PUT"); // output register A
    return true;
}

// result in register a
bool CodeGenerator::add(Variable* var1, Variable* var2) {
   // addInstruction("( dodawanie " + std::to_string(var1->val) + " " + std::to_string(var2->val) + " )");
    // wynik mozna do jakiegos rejestry i wtedy z niego - chyba zrobione
    if (var1 != nullptr && var2 != nullptr) {
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value to a
        addInstruction("SWAP c"); // store in c
        makeConstant(var1->address); // in a
        addInstruction("LOAD a");
        addInstruction("ADD c"); // result in a
        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::subtract(Variable* var1, Variable* var2) {
    //addInstruction("( odejmowanie " + std::to_string(var1->val) + " " + std::to_string(var2->val) + " )");
    if (var1 != nullptr && var2 != nullptr) {
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value to a
        addInstruction("SWAP c"); // store in c
        makeConstant(var1->address); // in a
        addInstruction("LOAD a");
        addInstruction("SUB c"); // result in a
        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::multiply(Variable* var1, Variable* var2) {
   // addInstruction("( mnozenie " + std::to_string(var1->val) + " " + std::to_string(var2->val) + " )");
    if (var1 != nullptr && var2 != nullptr) {
        // quick multiplying
        // makeConstant(var2->address); to raczej niepotrzebne operowac bedziemy na val
        // val of multiplier
        long long int multiplier = var2->val;
        long long int mul = var1->val;
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        addInstruction("PUT");
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("DEC e"); // e == -1
        addInstruction("RESET g");
        addInstruction("INC g");
        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("SWAP f"); // f == val1

        addInstruction("RESET h");
        addInstruction("SWAP h");
        addInstruction("ADD c");
        addInstruction("SWAP h");
        addInstruction("SWAP c"); // multiplier in register a
        addInstruction("JZERO " + std::to_string(15)); // jump if multiplier == 0
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        // check what was the shifted bit
        addInstruction("RESET a");
        addInstruction("ADD c"); // a == mulplier po shifcie
        addInstruction("SHIFT g"); // a == miltiplier << 1
        // check if equal
        addInstruction("SUB h"); // a = multiplier << 1 - poprzednia wartosc multiplier
        // if not equal then add to result
        addInstruction("JZERO 4");
        addInstruction("SWAP d");
        addInstruction("ADD f");
        addInstruction("SWAP d");
        // shift left mul
        addInstruction("SWAP f");
        addInstruction("SHIFT g");
        addInstruction("SWAP f");
        // jump to checking cond
        addInstruction("JUMP -" + std::to_string(19));
        addInstruction("SWAP d");
//        long long int prev;
//        while (multiplier != 0) {
//            prev = multiplier;
//            multiplier = multiplier >> 1;
//            addInstruction("SWAP c");
//            addInstruction("SHIFT e");
//            addInstruction("SWAP c");
//            // check what was the 'shifted' bit
//            long long int tmp = multiplier;
//            addInstruction("( multiplier " + std::to_string(multiplier) + " prev " + std::to_string(prev) + " )");
//            if (multiplier << 1 != prev) {
//                // 1 was shifted, add mul to result in register d
//                addInstruction("SWAP d");
//                addInstruction("ADD f");
//                addInstruction("SWAP d");
//            }
//            // shift left mul
//            mul = mul << 1;
//            addInstruction("( mul " + std::to_string(mul) + " )");
//            addInstruction("SWAP f");
//            addInstruction("SHIFT g");
//            addInstruction("SWAP f");
//        }
//        addInstruction("SWAP d");
        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::multiply(Variable* var1, Variable* var2) {
    // addInstruction("( mnozenie " + std::to_string(var1->val) + " " + std::to_string(var2->val) + " )");
    if (var1 != nullptr && var2 != nullptr) {
        // quick multiplying
        // makeConstant(var2->address); to raczej niepotrzebne operowac bedziemy na val
        // val of multiplier
        long long int multiplier = var2->val;
        long long int mul = var1->val;
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        addInstruction("PUT");
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("DEC e"); // e == -1
        addInstruction("RESET g");
        addInstruction("INC g");
        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("SWAP f"); // f == val1

        addInstruction("RESET h");
        addInstruction("SWAP h");
        addInstruction("ADD c");
        addInstruction("SWAP h");
        addInstruction("SWAP c"); // multiplier in register a
        addInstruction("JZERO " + std::to_string(15)); // jump if multiplier == 0
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        // check what was the shifted bit
        addInstruction("RESET a");
        addInstruction("ADD c"); // a == mulplier po shifcie
        addInstruction("SHIFT g"); // a == miltiplier << 1
        // check if equal
        addInstruction("SUB h"); // a = multiplier << 1 - poprzednia wartosc multiplier
        // if not equal then add to result
        addInstruction("JZERO 4");
        addInstruction("SWAP d");
        addInstruction("ADD f");
        addInstruction("SWAP d");
        // shift left mul
        addInstruction("SWAP f");
        addInstruction("SHIFT g");
        addInstruction("SWAP f");
        // jump to checking cond
        addInstruction("JUMP -" + std::to_string(19));
        addInstruction("SWAP d");
        return true;
    }
    return false;
}
#include "../inc/CodeGenerator.hpp"
#include "../inc/MemoryData.hpp"
#include "../inc/Variable.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <bitset>
#include <iostream>
#include <cstdlib>


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
    // clear register A
   // addInstruction("( const making " + std::to_string(val) + " )");
    addInstruction("RESET a");
    if (val == 0) {
        return 0;
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

Variable* CodeGenerator::allocateConstantNegate(long long int value) {
    long long int address = memo->addConstant(-value);
    if (address != -1) {
        // addInstruction("( const alloc" + std::to_string(value) + " )");
        makeConstant(address);
        addInstruction("SWAP c");
        // constant was allocated under address
        makeConstant(value); // result in register a
        addInstruction("SWAP b");
        addInstruction("RESET a");
        addInstruction("SUB b");
        addInstruction("STORE c");
    }
    return memo->getVar(std::to_string(-value));
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

void CodeGenerator::changeInstruction(long long int index, std::string newInstruction) {
    code[index] = newInstruction;
}

std::string CodeGenerator::getInstruction(long long int index) {
    return code[index];
}

// result in register a; if not equal then it shan't be zero
long long int CodeGenerator::evalNotEqual(Variable* var1, Variable* var2) {
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    return addInstruction("JZERO "); // later will be changed, return index of this instruction
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

bool CodeGenerator::read(Variable* var) {
    //makeConstant(val);
    // get from memory and load to a
    makeConstant(var->address);
    addInstruction("SWAP b"); // address of variable in register b
    addInstruction("GET"); // in register a
    var->isInit = true;
    addInstruction("STORE b");
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
        if (var1->val == 0 || var2->val == 0) {
            addInstruction("RESET a");
            return true;
        }
        if (var1->val == 1) {
            makeConstant(var2->address);
            addInstruction("LOAD a");
            return true;
        }
        if (var2->val == 1) {
            makeConstant(var1->address);
            addInstruction("LOAD a");
            return true;
        }
        bool negate = false;
        if ((var2->val < 0 && var1->val > 0) || (var2->val > 0 && var1->val < 0)) {
            negate = true;
        }
        // quick multiplying
        // makeConstant(var2->address); to raczej niepotrzebne operowac bedziemy na val
        // val of multiplier
        long long int multiplier = var2->val;
        long long int mul = var1->val;
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        addInstruction("SWAP c"); // store in c, c == multiplier
        if (var2->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB c");
            addInstruction("SWAP c");
        }
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("DEC e"); // e == -1
        addInstruction("RESET g");
        addInstruction("INC g");
        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("SWAP f"); // f == val1
        if (var1->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB f");
            addInstruction("SWAP f");
        }

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
        if (negate) {
            addInstruction("RESET a");
            addInstruction("SUB d");
           // addInstruction("SWAP d");
        } else {
            addInstruction("SWAP d"); // res is now in a
        }
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
bool CodeGenerator::divide(Variable* var1, Variable* var2) {
    // TODO  poteg 2 mozna pomylslec zeby tylko byly shifty
    // TODO sprawdzic czy zwkle robienie stalej bardziej nieoplacalne
    if (var1 != nullptr && var2 != nullptr) {
        // quick dividing
        if (var1->val == 0 || var2->val == 0 || std::abs(var1->val) < std::abs(var2->val)) {
            addInstruction("RESET a");
            return true;
        }
        if (var2->val == 1 || var2->val == -1) {
            makeConstant(var1->address);
            addInstruction("LOAD a"); // value1 to a
            if (var2->val == -1 && var1->val > 0) {
                // negate result
                addInstruction("SWAP b");
                addInstruction("RESET a");
                addInstruction("SUB b");
            }
            return true;
        }

        // sign of result
        bool negate = false;
        if ((var2->val < 0 && var1->val > 0) || (var2->val > 0 && var1->val < 0)) {
            negate = true;
        }

        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        addInstruction("SWAP c"); // store in c, c == var2
        if (var2->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB c");
            addInstruction("SWAP c");
        }
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("INC e"); // e == -1
        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("SWAP f"); // f == val1
        if (var1->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB f");
            addInstruction("SWAP f");
        }

        // find number of bits in var1 (var1 > var2)
        int counter1 = 0;
        long long int tmp = var1->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter1++;
        }

        int counter2 = 0;
        tmp = var2->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter2++;
        }

        int counter3 = 0;
        tmp = var1->val / var2->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter3++;
        }

        // counter1 in h
        makeConstant(counter3);
       // addInstruction("PUT");
        addInstruction("SWAP h");

        // align
        if (counter1 != counter2) {
            // shift left var2 by difference
            makeConstant(counter1 - counter2);
            addInstruction("SWAP b");
            addInstruction("SWAP c");
            addInstruction("SHIFT b");
            addInstruction("SWAP c"); // in c now var2 is aligned
        }


        // algorithm: Q = N/D
        // 1. align MSB of n AND d
        // for i in 1::number of bits in result
        // 2. compute t = (N-D)
        // 3. if (t >= 0) add to Q one and N=t
        // 4. left shift N by 1
        // 5. Left-shift Q by 1
        // 6. go to 2.
        // shift right Q to undo step 1.
        addInstruction("SWAP h");
        addInstruction("JZERO 16");
        addInstruction("SWAP h");
        // make copy of f in a
        addInstruction("RESET a");
        addInstruction("ADD f");
        //addInstruction("SWAP f");
        addInstruction("SUB c");
        // check if t>=0
        addInstruction("JNEG 3"); // t < 0 don't set N = t and don't add +1 to Q
        // increment Q
        addInstruction("INC d");
       // addInstruction("JZERO 10");
        addInstruction("SWAP f"); // N = t
        // left N = t and left shift N by 1
        addInstruction("SWAP f"); // N in a
        addInstruction("SHIFT e");
        //addInstruction("PUT");
        addInstruction("SWAP f");
        // left shift Q by 1
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("DEC h");
        addInstruction("JUMP -16");
        // finish
        // right shift Q by 1
        addInstruction("RESET a");
        addInstruction("DEC a");

        addInstruction("SWAP b");
        addInstruction("SWAP d");
        addInstruction("SHIFT b");
        addInstruction("SWAP d");
        if (negate) {
            addInstruction("RESET a");
            addInstruction("SUB d");
        } else {
            addInstruction("SWAP d"); // res is now in a
        }

        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::modulo(Variable* var1, Variable* var2) {
    if (var1 != nullptr && var2 != nullptr) {
        // quick modulo
        if (var1->val == 0 || var2->val == 0) {
            addInstruction("RESET a");
            return true;
        }

        if (std::abs(var1->val) < std::abs(var2->val)) { // remainder is var1 (take into account sign of var2)
            makeConstant(var1->address);
            addInstruction("LOAD a"); // value1 to a
            if ((var2->val < 0 && var1->val > 0) || (var2->val > 0 && var1->val < 0)) { // if var1 < 0 then it already has correct sign
                // negate result
                addInstruction("SWAP b");
                addInstruction("RESET a");
                addInstruction("SUB b");
            }
            return true;
        }

        if (var2->val == 1 || var2->val == -1) {
            makeConstant(var1->address);
            addInstruction("LOAD a"); // value1 to a
            if (var2->val < 0) {
                // negate result
                addInstruction("SWAP b");
                addInstruction("RESET a");
                addInstruction("SUB b");
            }
            return true;
        }

        // sign of result
        bool negate = false;
        if (var2->val < 0) {
            negate = true;
        }

        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        addInstruction("SWAP c"); // store in c, c == var2
        if (var2->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB c");
            addInstruction("SWAP c");
        }
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("INC e"); // e == -1
        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("SWAP f"); // f == val1
        if (var1->val < 0) {
            addInstruction("RESET a");
            addInstruction("SUB f");
            addInstruction("SWAP f");
        }

        // find number of bits in var1 (var1 > var2)
        int counter1 = 0;
        long long int tmp = var1->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter1++;
        }

        int counter2 = 0;
        tmp = var2->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter2++;
        }

        int counter3 = 0;
        tmp = var1->val / var2->val;
        if (tmp < 0) {
            tmp = -tmp;
        }
        while (tmp >= 1) {
            tmp = tmp >> 1;
            counter3++;
        }

        // counter1 in h
        makeConstant(counter3);
        addInstruction("SWAP h");

        // align
        if (counter1 != counter2) {
            // shift left var2 by difference
            makeConstant(counter1 - counter2);
            addInstruction("SWAP b");
            addInstruction("SWAP c");
            addInstruction("SHIFT b");
            addInstruction("SWAP c"); // in c now var2 is aligned
        }

        addInstruction("SWAP h");
        addInstruction("JZERO 16");
        addInstruction("SWAP h");
        // make copy of f in a
        addInstruction("RESET a");
        addInstruction("ADD f");
        //addInstruction("SWAP f");
        addInstruction("SUB c");
        // check if t>=0
        addInstruction("JNEG 3"); // t < 0 don't set N = t and don't add +1 to Q
        // increment Q
        addInstruction("INC d");
        // addInstruction("JZERO 10");
        addInstruction("SWAP f"); // N = t
        // left N = t and left shift N by 1
        addInstruction("SWAP f"); // N in a
        addInstruction("SHIFT e");
        //addInstruction("PUT");
        addInstruction("SWAP f");
        // left shift Q by 1
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("DEC h");
        addInstruction("JUMP -16");
        // finish
        // right shift N by 1
        addInstruction("RESET a");
        addInstruction("DEC a");

        addInstruction("SWAP b");
        addInstruction("SWAP f");
        addInstruction("SHIFT b");
        addInstruction("SWAP f");
        if (negate) {
            addInstruction("RESET a");
            addInstruction("SUB f");
        } else {
            addInstruction("SWAP f"); // res is now in a
        }

        return true;
    }
    return false;
}
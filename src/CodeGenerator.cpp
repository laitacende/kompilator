#include "../inc/CodeGenerator.hpp"
#include "../inc/MemoryData.hpp"
#include "../inc/Variable.hpp"
#include "../inc/Cond.hpp"

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
    //if (address != -1) { // problem jak kod w ifie, wtedy stala ma zla wartosc, bo nie zostala obliczona
       // addInstruction("( const alloc" + std::to_string(value) + " )");
        makeConstant(address);
        addInstruction("SWAP c");
        // constant was allocated under address
        makeConstant(value); // result in register a
        addInstruction("STORE c");
   // }
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
    return nullptr;
}

Variable* CodeGenerator::allocateArray(std::string name, long long int start, long long int end) {
    // check size
    if (start > end) {
        Variable* var = new Variable("error", 0); // incorrect range
        return var;
    }
    // determine size
    long long int size = std::abs(end - start) + 1;
    //std::cout << " size " << size << std::endl;
    // create variable
    long long int address = memo->addVariableOfSize(name, size, start, end);
    if (address != -1) {
        return memo->getVar(name);
    }
    return nullptr;
}

bool CodeGenerator::assignToVariable(Variable* var1, Variable* var2) {
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

Variable* CodeGenerator::getVarArrayNum(Variable* var, long long int index) {
    // To wczytywac do jakiegos rejestru, najpierw a, pozniej b itp. i miec taki pseudo stos
    // check address in memory of element at index
    // first element address
    long long int address = var->address;
    // check if not out of bounds
    if (index > var->endArray || index < var->startArray) {
        return nullptr;
    }
    long long int addrElement = abs(index - var->startArray) + address;
    Variable* var1 = new Variable(var->name, addrElement);
    var->size = var->size;
    var->isArray = true;
    var->isInit = true;
    var->startArray = var->startArray;
    var->endArray = var->endArray;
    return var1;
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
// ------------------------------- CONDITIONS -----------------------------

// result in register a; if not equal then it shan't be zero
Cond* CodeGenerator::evalNotEqual(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    Cond* c = new Cond(addInstruction("JZERO "), "NEQ");
    c->firstIndex = first;
    return c; // later will be changed, return index of this instruction
}

Cond* CodeGenerator::evalEqual(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JPOS ");
    Cond* c = new Cond(addInstruction("JNEG "), "EQ");
    c->firstIndex = first;
    return c; // later will be changed, return index of this instruction
}

Cond* CodeGenerator::evalLess(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JZERO ");
    Cond* c = new Cond(addInstruction("JPOS "), "LE");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalLessEqual(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    Cond* c = new Cond(addInstruction("JPOS "), "LEQ");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalGreater(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
  //  std::cout<<first<< " " << code[offset - 1] << std::endl;
    makeConstant(var2->address);
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JZERO ");
    Cond* c = new Cond(addInstruction("JNEG "), "GE");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalGreaterEqual(Variable* var1, Variable* var2) {
    long long int first = offset + 1;
    makeConstant(var2->address);
   // std::cout << var2->address << " var2addr" << std::endl;
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
   // std::cout << var1->address << " var1addr" << std::endl;
    addInstruction("LOAD a");

    addInstruction("SUB c");
    Cond* c = new Cond(addInstruction("JNEG "), "GEQ");
    c->firstIndex = first;
    return c;
}

// ----------------------------------- OPERATIONS -------------------------------

bool CodeGenerator::write(Variable* var) {
    // get from memory and load to a
    if (var != nullptr && var->isVariable && !var->isArray && !var->isInit) {
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
    if (var != nullptr) {
        makeConstant(var->address);
        addInstruction("SWAP b"); // address of variable in register b
        addInstruction("GET"); // in register a
        var->isInit = true;
        addInstruction("STORE b");
        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::add(Variable* var1, Variable* var2) {
   // addInstruction("( dodawanie " + std::to_string(var1->val) + " " + std::to_string(var2->val) + " )");
    // wynik mozna do jakiegos rejestry i wtedy z niego - chyba zrobione
    if (var1 != nullptr && var2 != nullptr) {
        if (var1->isConstant && var2->isConstant) {
            makeConstant(var1->val + var2->val);
        } else if (var2->isConstant && var1->isVariable && (var2->val == 0 || var2->val == 1)) {
            if (var2->val == 0) {
                makeConstant(var1->address); // in a
                addInstruction("LOAD a");
            } else {
                makeConstant(var1->address); // in a
                addInstruction("LOAD a");
                addInstruction("INC a");
            }
            return true;
        }
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
        if (var1->isConstant && var2->isConstant) {
            makeConstant(var1->val - var2->val);
        } else if (var2->isConstant && var1->isVariable && (var2->val == 0 || var2->val == 1)) {
            if (var2->val == 0) {
                makeConstant(var1->address); // in a
                addInstruction("LOAD a");
            } else {
                makeConstant(var1->address); // in a
                addInstruction("LOAD a");
                addInstruction("DEC a");
            }
            return true;
        }
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
        if (var1->isConstant && var2->isConstant) {
            if (var1->val == 0 || var2->val == 0) {
                addInstruction("RESET a");
            } else {
                bool negate = false;
                long long int tmp1 = var1->val;
                long long int tmp2 = var2->val;
                if ((var1->val > 0 && var2->val < 0) || (var1->val < 0 && var2->val > 0)) {
                    negate = true;
                    if (tmp1 < 0) {
                        tmp1 = -tmp1;
                    }
                    if (tmp2 < 0) {
                        tmp2 = -tmp2;
                    }
                }
                makeConstant(tmp1 * tmp2);

                if (negate) {
                    addInstruction("SWAP b");
                    addInstruction("RESET a");
                    addInstruction("SUB b");
                }
            }

            return true;
        }
//        if (var1->val == 0 || var2->val == 0) {
//            addInstruction("RESET a");
//            return true;
//        }
//        if (var1->val == 1) {
//            makeConstant(var2->address);
//            addInstruction("LOAD a");
//            return true;
//        }
//        if (var2->val == 1) {
//            makeConstant(var1->address);
//            addInstruction("LOAD a");
//            return true;
//        }
//        bool negate = false;
//        if ((var2->val < 0 && var1->val > 0) || (var2->val > 0 && var1->val < 0)) {
//            negate = true;
//        }
        // quick multiplying

        addInstruction("RESET d");
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        // check if is negative
        addInstruction("JPOS 5"); // if positive don't do this
        // +1 to register d - negate result
        addInstruction("INC d"); // var2 was negative
        // make positive
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET a");
        addInstruction("SUB c");
        addInstruction("SWAP c"); // positive var2 in register c

        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("JPOS 5"); // var1  > 0
        addInstruction("SWAP f"); // f == val1
        // check if var1 is negative
        // -1 to register d
        addInstruction("DEC d"); // var1 was negative
        // make positive
        addInstruction("RESET a");
        addInstruction("SUB f");
        addInstruction("SWAP f"); // positive var2 in register c

        addInstruction("SWAP d");
        addInstruction("SWAP b"); // if b != 0 then negate
        addInstruction("RESET d"); // result in d, d == 0
        addInstruction("RESET e");
        addInstruction("DEC e"); // e == -1
        addInstruction("RESET g");
        addInstruction("INC g");
//        if (var1->val < 0) {
//            addInstruction("RESET a");
//            addInstruction("SUB f");
//            addInstruction("SWAP f");
//        }

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
        addInstruction("SWAP b");
        addInstruction("JNEG 4"); // negate
        addInstruction("JPOS 3"); // negate
        addInstruction("SWAP d");
        addInstruction("JUMP 3");
        addInstruction("RESET a");
        addInstruction("SUB d");

//        if (negate) {
//            addInstruction("RESET a");
//            addInstruction("SUB d");
//           // addInstruction("SWAP d");
//        } else {
//            addInstruction("SWAP d"); // res is now in a
//        }
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
    if (var1 != nullptr && var2 != nullptr) {
        if (var1->isConstant && var2->isConstant) {
            if (var2->val == 0 || var1->val == 0) {
                addInstruction("RESET a");
            } else {
                bool negate = false;
                long long int tmp1 = var1->val;
                long long int tmp2 = var2->val;
                if ((var1->val > 0 && var2->val < 0) || (var1->val < 0 && var2->val > 0)) {
                    negate = true;
                    if (tmp1 < 0) {
                        tmp1 = -tmp1;
                    }
                    if (tmp2 < 0) {
                        tmp2 = -tmp2;
                    }
                }
                makeConstant(tmp1 / tmp2);
                if (negate) {
                    addInstruction("SWAP b");
                    addInstruction("RESET a");
                    addInstruction("SUB b");
                }
            }
            return true;
        }
        addInstruction("RESET d"); // result
        addInstruction("RESET g");
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        long long int offset1 = addInstruction("JZERO ");
        addInstruction("JPOS 5"); // if positive don't do this
        // +1 to register d - negate result
        addInstruction("INC g"); // var2 was negative
        // make positive
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET a");
        addInstruction("SUB c");
        addInstruction("SWAP c"); // positive var2 in register c


        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("JPOS 5"); // var1  > 0
        addInstruction("SWAP f"); // f == val1
        // check if var1 is negative
        // -1 to register g
        addInstruction("DEC g"); // var1 was negative
        // make positive
        addInstruction("RESET a");
        addInstruction("SUB f");
        addInstruction("SWAP f"); // positive var1 in register f

        addInstruction("RESET e");
        addInstruction("INC e");
        addInstruction("RESET h");
        addInstruction("DEC h");
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SWAP b"); // v3 (tmp)

        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f");
        addInstruction("JPOS 37"); //to end, v2>v1, result is 0, remainder is v1 (jakis swap albo cos)

        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2- v1
        addInstruction("JPOS 5");
       // addInstruction("JNEG 5"); // skip this, ldivpre
        addInstruction("SWAP c");
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        addInstruction("JUMP -7");

        // ldivpre
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");
        addInstruction("JUMP 16"); // ldivcond

        // ldivp1
        addInstruction("SWAP f");
        addInstruction("SUB c");
        addInstruction("SWAP f");

        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("INC a");
        addInstruction("SWAP d");

        // ldivp2
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB b");
        addInstruction("JZERO 14");
        addInstruction("JNEG 13"); // end
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");

        // ldivcond
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2 - v1 <=0
        addInstruction("JZERO -18"); // ldivp1
        addInstruction("JNEG -19"); // ldivp1
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("JUMP -16"); // ldivp2

        long long int offset2 = addInstruction("SWAP g");
        addInstruction("JNEG 4"); // negate
        addInstruction("JPOS 3"); // negate
        addInstruction("SWAP d");
        addInstruction("JUMP 3");
        addInstruction("RESET a");
        addInstruction("SUB d");

        changeInstruction(offset1, "JZERO " + std::to_string(offset2 - offset1));

        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::modulo(Variable* var1, Variable* var2) {
    if (var1 != nullptr && var2 != nullptr) {
        // quick modulo
        addInstruction("RESET d"); // result
        addInstruction("RESET g");
        addInstruction("RESET f");
        addInstruction("INC g");
        makeConstant(var2->address);
        addInstruction("LOAD a"); // value2 to a
        long long int offset1 = addInstruction("JZERO ");
        addInstruction("JPOS 5"); // if positive don't do this
        // +1 to register d - negate result
        addInstruction("DEC g"); // var2 was negative, if g == 0 then negate result
        // make positive
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET a");
        addInstruction("SUB c");
        addInstruction("SWAP c"); // positive var2 in register c


        makeConstant(var1->address); // in a
        addInstruction("LOAD a"); // a == val1
        addInstruction("JPOS 4"); // var1  > 0
        addInstruction("SWAP f"); // f == val1
        // check if var1 is negative
        // make positive
        addInstruction("RESET a");
        addInstruction("SUB f");
        addInstruction("SWAP f"); // positive var1 in register f

        addInstruction("RESET e");
        addInstruction("INC e");
        addInstruction("RESET h");
        addInstruction("DEC h");
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SWAP b"); // v3 (tmp)

        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f");
        addInstruction("JPOS 37"); //to end, v2>v1, result is v1, remainder is v1

        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2- v1
        addInstruction("JPOS 5");
        // addInstruction("JNEG 5"); // skip this, ldivpre
        addInstruction("SWAP c");
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        addInstruction("JUMP -7");

        // ldivpre
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");
        addInstruction("JUMP 16"); // ldivcond

        // ldivp1
        addInstruction("SWAP f");
        addInstruction("SUB c");
        addInstruction("SWAP f");

        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("INC a");
        addInstruction("SWAP d");

        // ldivp2
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB b");
        addInstruction("JZERO 14");
        addInstruction("JNEG 13"); // end
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");

        // ldivcond
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2 - v1 <=0
        addInstruction("JZERO -18"); // ldivp1
        addInstruction("JNEG -19"); // ldivp1
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("JUMP -16"); // ldivp2

        long long int offset2 = addInstruction("SWAP g");
        addInstruction("JZERO 3"); // negate
        addInstruction("SWAP f");
        addInstruction("JUMP 3");
        addInstruction("RESET a");
        addInstruction("SUB f");

        changeInstruction(offset1, "JZERO " + std::to_string(offset2 - offset1));

        return true;
    }
    return false;
}
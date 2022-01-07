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
#include <cmath>


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
    std::string binary = std::bitset<sizeof(long long int) * 8>(val).to_string();
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

Variable* CodeGenerator::allocateIterator(std::string name) {
    long long int address = memo->addVariable(name);
   // std::cout << "name it: " << name << " address " << address << std::endl;
    // check if variable of this name exists
    if (address == -1) { // there is variable of such name, copy io to temporary memo
        // check if this variable isn't iterator too
        Variable* var = memo->getVar(name);
        if (var->isIterator) {
            return nullptr;
        }
        memo->copyVarToTemp(name); // original variable in temp
        memo->eraseVariable(name);
        memo->addVariable(name);
    }
    Variable* var = memo->getVar(name); // TODO INIT ITERATOR WITH CORRECT VALUE
    var->isIterator = true;
    var->isInit = true;
    return var;
}

Variable* CodeGenerator::allocateArray(std::string name, long long int start, long long int end) {
    // check size
    if (start > end) {
        Variable* var = new Variable("error", 0); // incorrect range
        return var;
    }
    // determine size
    long long int size = std::abs(end - start) + 1;
    if (start < 0 && end < 0) {
        size--;
    }
    //std::cout << " size " << size << std::endl;
    // create variable
    long long int address = memo->addVariableOfSize(name, size, start, end);
    if (address != -1) {
        return memo->getVar(name);
    }
    return nullptr;
}

bool CodeGenerator::restoreVariable(std::string name) {
    memo->eraseVariable(name);
    memo->copyVarToMem(name);
    return true;
}

bool CodeGenerator::initializeIterator(Variable* iterator, Variable* from, Variable* to) {
    // if end is constant then just add its address to iterator
    makeConstant(iterator->address);
    addInstruction("SWAP g");
    makeConstant(from->address);
    // if its array with variable
    if (from->isArray && from->isArrayWithVar) { // in register c address of first element in array
    addInstruction("SWAP c");
    // load address of index
    makeConstant(from->offsetStack.top());
    from->offsetStack.pop();
    // load value of this variable
    addInstruction("LOAD a");
    // calculate offset in array (how many cells of memory to 'jump'
    addInstruction("SWAP f"); // value of variable
    long long int start = from->startArray;
    if (start < 0) {
    start = -start;
    }
    makeConstant(start); // has to be positive
    addInstruction("SWAP f"); // in a value of var, in f start of array
    if (from->startArray < 0) { // add
    addInstruction("ADD f"); // index - (-start)
    } else {
    addInstruction("SUB f"); // index - start
    }
    // add address of array's first element
    addInstruction("ADD c"); // index - start + address - this is address of element in array
    // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load from value
    addInstruction("STORE g"); // start value is now in iterator memory address

    if (!to->isConstant) {
        // copy to new memory cell
        makeConstant(memo->currentAddress); // new address
        iterator->toIterator = memo->currentAddress;
        memo->currentAddress = memo->currentAddress + 1; // allocate new constant
        addInstruction("SWAP c");
        makeConstant(to->address);
        addInstruction("LOAD a");
        addInstruction("STORE c");
    } else {
        iterator->toIterator = to->address;
    }
    iterator->toIteratorVar = to;
    return true;
}

long long int CodeGenerator::generateFor(Variable* iterator, Variable* from, Variable* to, bool isDown) {

    // load current iterator value
    makeConstant(iterator->address);
    addInstruction("LOAD a");
    addInstruction("SWAP c");

    if (from->isConstant && to->isConstant && std::abs(from->val - to->val) <= 7) { // loop unwinding
        iterator->times = std::abs(from->val - to->val);
        return -1;
    } else {

        // load current value of end condition and save it
        makeConstant(iterator->toIterator);
        if (iterator->toIteratorVar->isArray &&
            iterator->toIteratorVar->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP d");
            // load address of index
            makeConstant(iterator->toIteratorVar->offsetStack.top());
            iterator->toIteratorVar->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = iterator->toIteratorVar->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (iterator->toIteratorVar->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD d"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
        addInstruction("LOAD a");
        addInstruction("SUB c");
        if (isDown) {
            return addInstruction("JPOS ");
        }
        return addInstruction("JNEG "); // modify later
    }
}

bool CodeGenerator::unwindLoop(long long int then, Variable* iterator) {
    // copy instruction from then to last instruction
    int times = iterator->times;
    long long int end = offset;
    long long int start = then;
    for (int i = 0; i < times; i++) {
        start = then;
        while (start < end) {
            addInstruction(code[start]);
            start++;
        }
    }
    return true;
}

bool CodeGenerator::modifyIterator(Variable* iterator, bool isDown) {
    // load iterator value and address
    //std::cout << "modify it address name" << iterator->address << "  " << iterator->name << std::endl;
    makeConstant(iterator->address);

    addInstruction("SWAP c");
    addInstruction("RESET a");
    addInstruction("ADD c");
    addInstruction("LOAD a");
    if (isDown) {
        addInstruction("DEC a");
    } else {
        addInstruction("INC a");
    }
    addInstruction("STORE c"); // load new value of iterator
    return true;
}

bool CodeGenerator::assignToVariable(Variable* var1, Variable* var2) {
    // TODO mozna wczytywac do rejestrow,a nie sa dwa razy do pamieci odwolywac
    if (var1 != nullptr && var2 != nullptr) {
        var1->isInit = true;
        var1->val = var2->val;
        // result of expression is in a
        //addInstruction("( assign to " + var1->name + " val " + std::to_string(var2->val) + " )");
        addInstruction("SWAP d"); // result of expression is in d
        // handle arrays with variables as indices
        // var1 has address of first entry in array in memory and of index
        makeConstant(var1->address); // in register a
        addInstruction("SWAP c");
        if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
            // load address of index
            makeConstant(var1->offsetStack.top());
           // std::cout << "offset " << var1->offsetStack.top() << std::endl;
            var1->offsetStack.pop();

            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
//            } else if (var1->startArray == 0) {
//                addInstruction("INC a");
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            addInstruction("SWAP c"); // new address in register c
        }
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
    long long int addrElement = std::abs(index - var->startArray) + address;
    Variable* var1 = new Variable(var->name, addrElement);
    var->size = var->size;
    var->isArray = true;
    var->isInit = true;
   // std::cout << addrElement << " addr " << index << std::endl;
    var->startArray = var->startArray;
    var->endArray = var->endArray;
    return var1;
}

bool CodeGenerator::getConstant(std::string name) {
    if (memo->ifExists(name)) {
        long long int address = memo->getAddress(name);

        return true;
    }
    return false;
}

void CodeGenerator::changeInstruction(long long int index, std::string newInstruction) {
    code[index] = newInstruction;
}

std::string CodeGenerator::getInstruction(long long int index) {
    return code[index];
}

// get variable value to register a
bool CodeGenerator::loadVar(Variable* var) {
    makeConstant(var->address);
    if (var->isArray && var->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var->offsetStack.top());
        var->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a");
    return true;
}
// ------------------------------- CONDITIONS -----------------------------

// result in register a; if not equal then it shan't be zero
Cond* CodeGenerator::evalNotEqual(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val != var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else if (var1->val == var2->val) {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
    makeConstant(var2->address);
    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
    addInstruction("LOAD a");
    addInstruction("SUB c");
    Cond* c = new Cond(addInstruction("JZERO "), "NEQ");
    c->firstIndex = first;
    return c; // later will be changed, return index of this instruction
}

Cond* CodeGenerator::evalEqual(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val == var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else if (var1->val != var2->val) {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
    makeConstant(var2->address);
    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JPOS ");
    Cond* c = new Cond(addInstruction("JNEG "), "EQ");
    c->firstIndex = first;
    return c; // later will be changed, return index of this instruction
}

Cond* CodeGenerator::evalLess(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val < var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
    makeConstant(var2->address);
    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JZERO ");
    Cond* c = new Cond(addInstruction("JPOS "), "LE");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalLessEqual(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val <= var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
    makeConstant(var2->address);

    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
    addInstruction("LOAD a");
    addInstruction("SUB c");
    Cond* c = new Cond(addInstruction("JPOS "), "LEQ");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalGreater(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val > var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
  //  std::cout<<first<< " " << code[offset - 1] << std::endl;
    makeConstant(var2->address);

    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
    addInstruction("LOAD a");
    addInstruction("SUB c");
    addInstruction("JZERO ");
    Cond* c = new Cond(addInstruction("JNEG "), "GE");
    c->firstIndex = first;
    return c;
}

Cond* CodeGenerator::evalGreaterEqual(Variable* var1, Variable* var2) {
    if (var1->isConstant && var2->isConstant) {
        if (var1->val >= var2->val) {
            // always true, don't jump
            Cond* c = new Cond(-1, "TRUE");
            return c;
        } else {
            // always false, just jump
            Cond* c = new Cond(addInstruction("JUMP "), "FALSE");
            return c;
        }
    }
    long long int first = offset + 1;
    makeConstant(var2->address);
    if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var2->offsetStack.top());
        var2->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var2->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var2->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
   // std::cout << var2->address << " var2addr" << std::endl;
    addInstruction("LOAD a"); // load var2
    addInstruction("SWAP c"); // var2 in c
    makeConstant(var1->address);
    if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
        addInstruction("SWAP d");
        // load address of index
        makeConstant(var1->offsetStack.top());
        var1->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var1->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var1->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD d"); // index - start + address - this is address of element in array
        // new address in register a
    }
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
   // std::cout << var->name << std::endl;
    if (var != nullptr && var->isVariable && !var->isArray && !var->isIterator && !var->isInit) {
        return false;
    }
    makeConstant(var->address);
    //std::cout << "write addr " << var->address << std::endl;
    // check if var isn't array indexed by variable
    if (var->isArray && var->isArrayWithVar) { // in register c address of first element in array
        addInstruction("SWAP c");
        // load address of index
        makeConstant(var->offsetStack.top());
        var->offsetStack.pop();
        // load value of this variable
        addInstruction("LOAD a");
        // calculate offset in array (how many cells of memory to 'jump'
        addInstruction("SWAP f"); // value of variable
        long long int start = var->startArray;
        if (start < 0) {
            start = -start;
        }
        makeConstant(start); // has to be positive
        addInstruction("SWAP f"); // in a value of var, in f start of array
        if (var->startArray < 0) { // add
            addInstruction("ADD f"); // index - (-start)
        } else {
            addInstruction("SUB f"); // index - start
        }
        // add address of array's first element
        addInstruction("ADD c"); // index - start + address - this is address of element in array
        // addInstruction("SWAP c"); // new address in register a
    }
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
        if (var->isArray && var->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var->offsetStack.top());
            var->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
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
        } else if (var2->isConstant && !var1->isConstant && (var2->val == 0 || var2->val == 1 || var2->val == -1)) {
            makeConstant(var1->address); // in a
            if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
                addInstruction("SWAP c");
                // load address of index
                makeConstant(var1->offsetStack.top());
                var1->offsetStack.pop();
                // load value of this variable
                addInstruction("LOAD a");
                // calculate offset in array (how many cells of memory to 'jump'
                addInstruction("SWAP f"); // value of variable
                long long int start = var1->startArray;
                if (start < 0) {
                    start = -start;
                }
                makeConstant(start); // has to be positive
                addInstruction("SWAP f"); // in a value of var, in f start of array
                if (var1->startArray < 0) { // add
                    addInstruction("ADD f"); // index - (-start)
                } else {
                    addInstruction("SUB f"); // index - start
                }
                // add address of array's first element
                addInstruction("ADD c"); // index - start + address - this is address of element in array
                // addInstruction("SWAP c"); // new address in register a
            }
            addInstruction("LOAD a");
            if (var2->val == 1) {
                addInstruction("INC a");
            } else if (var2->val == -1) {
                addInstruction("DEC a");
            }
            return true;
        } else if (var1->isConstant && !var2->isConstant && (var1->val == 0 || var1->val == 1 || var1->val == -1)) {
            makeConstant(var2->address); // in a
            if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
                addInstruction("SWAP c");
                // load address of index
                makeConstant(var2->offsetStack.top());
                var2->offsetStack.pop();
                // load value of this variable
                addInstruction("LOAD a");
                // calculate offset in array (how many cells of memory to 'jump'
                addInstruction("SWAP f"); // value of variable
                long long int start = var2->startArray;
                if (start < 0) {
                    start = -start;
                }
                makeConstant(start); // has to be positive
                addInstruction("SWAP f"); // in a value of var, in f start of array
                if (var2->startArray < 0) { // add
                    addInstruction("ADD f"); // index - (-start)
                } else {
                    addInstruction("SUB f"); // index - start
                }
                // add address of array's first element
                addInstruction("ADD c"); // index - start + address - this is address of element in array
                // addInstruction("SWAP c"); // new address in register a
            }
            addInstruction("LOAD a");
            if (var1->val == 1) {
                addInstruction("INC a");
            } else if (var1->val == -1) {
                addInstruction("DEC a");
            }
            return true;
        }
        makeConstant(var2->address);
        if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var2->offsetStack.top());
            var2->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var2->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var2->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
           // addInstruction("SWAP c"); // new address in register a
        }
            addInstruction("LOAD a"); // value to a
            addInstruction("SWAP c"); // store in c
            makeConstant(var1->address); // in a
            // check if var1 is also array with variable as index
        if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
            addInstruction("SWAP d");
            // load address of index
            makeConstant(var1->offsetStack.top());
            var1->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD d"); // index - start + address - this is address of element in array
             // new address in register a
        }
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
        } else if (var2->isConstant && !var1->isConstant && (var2->val == 0 || var2->val == 1 || var2->val == -1)) {
            makeConstant(var1->address); // in a
            if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
                addInstruction("SWAP c");
                // load address of index
                makeConstant(var1->offsetStack.top());
                var1->offsetStack.pop();
                // load value of this variable
                addInstruction("LOAD a");
                // calculate offset in array (how many cells of memory to 'jump'
                addInstruction("SWAP f"); // value of variable
                long long int start = var1->startArray;
                if (start < 0) {
                    start = -start;
                }
                makeConstant(start); // has to be positive
                addInstruction("SWAP f"); // in a value of var, in f start of array
                if (var1->startArray < 0) { // add
                    addInstruction("ADD f"); // index - (-start)
                } else {
                    addInstruction("SUB f"); // index - start
                }
                // add address of array's first element
                addInstruction("ADD c"); // index - start + address - this is address of element in array
                // addInstruction("SWAP c"); // new address in register a
            }
            addInstruction("LOAD a");
            if (var2->val == 1) {
                addInstruction("DEC a");
            } else if (var2->val == -1) {
                addInstruction("INC a");
            }
            return true;
        } else if (var1->isConstant && !var2->isConstant && (var1->val == 0 || var1->val == 1 || var1->val == -1)) {
            makeConstant(var2->address); // in a
            if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
                addInstruction("SWAP c");
                // load address of index
                makeConstant(var2->offsetStack.top());
                var2->offsetStack.pop();
                // load value of this variable
                addInstruction("LOAD a");
                // calculate offset in array (how many cells of memory to 'jump'
                addInstruction("SWAP f"); // value of variable
                long long int start = var2->startArray;
                if (start < 0) {
                    start = -start;
                }
                makeConstant(start); // has to be positive
                addInstruction("SWAP f"); // in a value of var, in f start of array
                if (var2->startArray < 0) { // add
                    addInstruction("ADD f"); // index - (-start)
                } else {
                    addInstruction("SUB f"); // index - start
                }
                // add address of array's first element
                addInstruction("ADD c"); // index - start + address - this is address of element in array
                // addInstruction("SWAP c"); // new address in register a
            }
            addInstruction("LOAD a");
            if (var1->val == 1) {
                addInstruction("DEC a");
            } else if (var1->val == -1) {
                addInstruction("INC a");
            }
            return true;
        }
            makeConstant(var2->address);
        if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var2->offsetStack.top());
            var2->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var2->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var2->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
            addInstruction("LOAD a"); // value to a
            addInstruction("SWAP c"); // store in c
            makeConstant(var1->address); // in a

        if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
            addInstruction("SWAP d");
            // load address of index
            makeConstant(var1->offsetStack.top());
            var1->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD d"); // index - start + address - this is address of element in array
            // new address in register a
        }
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
        } else if (var1->isConstant && !var2->isConstant && (var1->val == 0 || var1->val == 1 || var1->val == -1
            || std::floor(std::log2(var1->val)) == std::ceil(std::log2(var1->val)))) {
            if (var1->val == 0) {
                addInstruction("RESET a");
            } else {
                // load var2
                makeConstant(var2->address); // in a
                if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
                    addInstruction("SWAP c");
                    // load address of index
                    makeConstant(var2->offsetStack.top());
                    var2->offsetStack.pop();
                    // load value of this variable
                    addInstruction("LOAD a");
                    // calculate offset in array (how many cells of memory to 'jump'
                    addInstruction("SWAP f"); // value of variable
                    long long int start = var2->startArray;
                    if (start < 0) {
                        start = -start;
                    }
                    makeConstant(start); // has to be positive
                    addInstruction("SWAP f"); // in a value of var, in f start of array
                    if (var2->startArray < 0) { // add
                        addInstruction("ADD f"); // index - (-start)
                    } else {
                        addInstruction("SUB f"); // index - start
                    }
                    // add address of array's first element
                    addInstruction("ADD c"); // index - start + address - this is address of element in array
                    // addInstruction("SWAP c"); // new address in register a
                }
                addInstruction("LOAD a");
                if (var1->val == -1) {
                    // negate var2
                    addInstruction("SWAP c");
                    addInstruction("RESET a");
                    addInstruction("SUB c");
                } else if (std::floor(std::log2(var1->val)) == std::ceil(std::log2(var1->val))) { // is power of 2
                    addInstruction("SWAP c"); // var2
                    addInstruction("RESET a");
                    addInstruction("INC a");
                    addInstruction("SWAP c");
                    long long int iter = std::floor(std::log2(var1->val));
                    while (iter != 0) {
                        iter--;
                        addInstruction("SHIFT c");
                    }
                }

                return true;
            }
        } else if (var2->isConstant && !var1->isConstant && (var2->val == 0 || var2->val == 1 || var2->val == -1 ||
                std::floor(std::log2(var2->val)) == std::ceil(std::log2(var2->val)))) {
            if (var2->val == 0) {
                addInstruction("RESET a");
            } else {
                // load var1
                makeConstant(var1->address); // in a
                if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
                    addInstruction("SWAP c");
                    // load address of index
                    makeConstant(var1->offsetStack.top());
                    var1->offsetStack.pop();
                    // load value of this variable
                    addInstruction("LOAD a");
                    // calculate offset in array (how many cells of memory to 'jump'
                    addInstruction("SWAP f"); // value of variable
                    long long int start = var1->startArray;
                    if (start < 0) {
                        start = -start;
                    }
                    makeConstant(start); // has to be positive
                    addInstruction("SWAP f"); // in a value of var, in f start of array
                    if (var1->startArray < 0) { // add
                        addInstruction("ADD f"); // index - (-start)
                    } else {
                        addInstruction("SUB f"); // index - start
                    }
                    // add address of array's first element
                    addInstruction("ADD c"); // index - start + address - this is address of element in array
                    // addInstruction("SWAP c"); // new address in register a
                }
                addInstruction("LOAD a");
                if (var2->val == -1) {
                    // negate var2
                    addInstruction("SWAP c");
                    addInstruction("RESET a");
                    addInstruction("SUB c");
                }  else if (std::floor(std::log2(var2->val)) == std::ceil(std::log2(var2->val))) { // is power of 2
                    addInstruction("SWAP c"); // var2
                    addInstruction("RESET a");
                    addInstruction("INC a");
                    addInstruction("SWAP c");
                    long long int iter = std::floor(std::log2(var2->val));
                    while (iter != 0) {
                        iter--;
                        addInstruction("SHIFT c");
                    }
                }
                return true;
            }
        }
        // quick multiplying
        addInstruction("RESET d");
        makeConstant(var2->address);
        if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var2->offsetStack.top());
            var2->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var2->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var2->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
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
        if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
            addInstruction("SWAP h");
            // load address of index
            makeConstant(var1->offsetStack.top());
            var1->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD h"); // index - start + address - this is address of element in array
            // new address in register a
        }
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
        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::divide(Variable* var1, Variable* var2) {
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
                makeConstant((tmp1 / tmp2));
                if (negate) {
                    addInstruction("INC a");
                    addInstruction("SWAP b");
                    addInstruction("RESET a");
                    addInstruction("SUB b");
                }
                return true;
            }
        } else if (!var1->isConstant && var2->isConstant && (var2->val == 1 || var2->val == -1)) {
            makeConstant(var1->address); // in a
            if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
                addInstruction("SWAP c");
                // load address of index
                makeConstant(var1->offsetStack.top());
                var1->offsetStack.pop();
                // load value of this variable
                addInstruction("LOAD a");
                // calculate offset in array (how many cells of memory to 'jump'
                addInstruction("SWAP f"); // value of variable
                long long int start = var1->startArray;
                if (start < 0) {
                    start = -start;
                }
                makeConstant(start); // has to be positive
                addInstruction("SWAP f"); // in a value of var, in f start of array
                if (var1->startArray < 0) { // add
                    addInstruction("ADD f"); // index - (-start)
                } else {
                    addInstruction("SUB f"); // index - start
                }
                // add address of array's first element
                addInstruction("ADD c"); // index - start + address - this is address of element in array
                // addInstruction("SWAP c"); // new address in register a
            }
            addInstruction("LOAD a");
            if (var2->val == -1) {
                // negate var2
                addInstruction("SWAP c");
                addInstruction("RESET a");
                addInstruction("SUB c");
            } else if (std::floor(std::log2(var2->val)) == std::ceil(std::log2(var2->val))) { // is power of 2
                addInstruction("SWAP c"); // var1
                addInstruction("RESET a");
                addInstruction("DEC a");
                addInstruction("SWAP c");
                long long int iter = std::floor(std::log2(var2->val));
                while (iter != 0) {
                    iter--;
                    addInstruction("SHIFT c");
                }
            }

            return true;
        }

        addInstruction("RESET d"); // result
        addInstruction("RESET g");
        makeConstant(var2->address);
        if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var2->offsetStack.top());
            var2->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var2->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var2->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
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
        if (var1->isArray && var1->isArrayWithVar) { // in register d address of first element in array
            addInstruction("SWAP d");
            // load address of index
            makeConstant(var1->offsetStack.top());
            var1->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP f"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP f"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD f"); // index - (-start)
            } else {
                addInstruction("SUB f"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD d"); // index - start + address - this is address of element in array
            // new address in register a
        }
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

        addInstruction("RESET d");
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
       // addInstruction("JNEG 5"); // skip this, block1
        addInstruction("SWAP c");
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        addInstruction("JUMP -7");

        // block1
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");
        addInstruction("JUMP 16"); // block4

        // block2
        addInstruction("SWAP f");
        addInstruction("SUB c");
        addInstruction("SWAP f");

        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("INC a");
        addInstruction("SWAP d");

        // block3
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB b");
        addInstruction("JZERO 14");
        addInstruction("JNEG 13"); // end
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");

        // block4
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2 - v1 <=0
        addInstruction("JZERO -18"); // block2
        addInstruction("JNEG -19"); // block2
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("JUMP -16"); // block3

        long long int offset2 = addInstruction("SWAP g");
        addInstruction("JNEG 4"); // negate
        addInstruction("JPOS 3"); // negate
        addInstruction("SWAP d");
        addInstruction("JUMP 4");
        addInstruction("RESET a");
        addInstruction("SUB d");
        addInstruction("DEC a");

        changeInstruction(offset1, "JZERO " + std::to_string(offset2 - offset1));

        return true;
    }
    return false;
}

// result in register a
bool CodeGenerator::modulo(Variable* var1, Variable* var2) {
    if (var1 != nullptr && var2 != nullptr) {
        if (var1->isConstant && var2->isConstant) {
            if (var2->val == 0 || var1->val == 0) {
                addInstruction("RESET a");
            } else {
                bool negate = false;
                long long int tmp1 = var1->val;
                long long int tmp2 = var2->val;
                if (var2->val < 0 || var1->val < 0) {
                    negate = true;
                    if (tmp1 < 0) {
                        tmp1 = -tmp1;
                    }
                    if (tmp2 < 0) {
                        tmp2 = -tmp2;
                    }
                }
                makeConstant(tmp1 % tmp2);
                if (negate) {
                    // if var1 negative and var2 positive = var2 - result (positive)
                    // if var1 positive and var2 negative = -(var2-result)
                    // if both negative result is just var1 % var2 but negative
                    if ((var1->val < 0 && var2->val > 0) || (var1->val > 0 && var2->val < 0)) {
                        // result in a
                        addInstruction("SWAP c"); // result in b
                        makeConstant(tmp2);
                        addInstruction("SUB c");
                        if (var1->val > 0 && var2->val < 0) {
                            // negate
                            addInstruction("SWAP b");
                            addInstruction("RESET a");
                            addInstruction("SUB b");
                        }
                    } else if (var1->val < 0 && var2->val < 0) {
                        addInstruction("SWAP b");
                        addInstruction("RESET a");
                        addInstruction("SUB b");
                    }
                }
                return true;
            }
        } else if (!var1->isConstant && var2->isConstant && (var2->val == 1 || var2->val == -1)) {
            addInstruction("RESET a");
            return true;
        }
        // quick modulo
        addInstruction("RESET d"); // result
        addInstruction("RESET g");
        addInstruction("RESET f");
        addInstruction("INC g");


        makeConstant(var2->address);
        if (var2->isArray && var2->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP c");
            // load address of index
            makeConstant(var2->offsetStack.top());
            var2->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP h"); // value of variable
            long long int start = var2->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP h"); // in a value of var, in f start of array
            if (var2->startArray < 0) { // add
                addInstruction("ADD h"); // index - (-start)
            } else {
                addInstruction("SUB h"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD c"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
        addInstruction("LOAD a"); // value2 to a
        long long int offset1 = addInstruction("JZERO ");
        addInstruction("RESET g");
        addInstruction("JPOS 5"); // if positive don't do this
        addInstruction("INC g"); // var2 was negative, g now is 1
        // make positive
        addInstruction("SWAP c"); // store in c, c == multiplier
        addInstruction("RESET a");
        addInstruction("SUB c");
        addInstruction("SWAP c"); // positive var2 in register c


        makeConstant(var1->address); // in a
        if (var1->isArray && var1->isArrayWithVar) { // in register c address of first element in array
            addInstruction("SWAP f");
            // load address of index
            makeConstant(var1->offsetStack.top());
            var1->offsetStack.pop();
            // load value of this variable
            addInstruction("LOAD a");
            // calculate offset in array (how many cells of memory to 'jump'
            addInstruction("SWAP h"); // value of variable
            long long int start = var1->startArray;
            if (start < 0) {
                start = -start;
            }
            makeConstant(start); // has to be positive
            addInstruction("SWAP h"); // in a value of var, in f start of array
            if (var1->startArray < 0) { // add
                addInstruction("ADD h"); // index - (-start)
            } else {
                addInstruction("SUB h"); // index - start
            }
            // add address of array's first element
            addInstruction("ADD f"); // index - start + address - this is address of element in array
            // addInstruction("SWAP c"); // new address in register a
        }
        addInstruction("LOAD a"); // a == val1
        addInstruction("JPOS 10"); // var1  > 0
        addInstruction("SWAP f"); // f == val1
        // check if var1 is negative
        addInstruction("RESET a");
        addInstruction("ADD g"); // if g == 1 then it means var2 < 0
        addInstruction("JZERO 3");
        addInstruction("INC g"); // g == 2, var1 < 0 and var2 < 0
        addInstruction("JUMP 2");
        addInstruction("DEC g"); // g == -1, var1 < 0 and var2 > 0

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
        // addInstruction("JNEG 5"); // skip this, block1
        addInstruction("SWAP c");
        addInstruction("SHIFT e");
        addInstruction("SWAP c");
        addInstruction("JUMP -7");

        // block1
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");
        addInstruction("JUMP 16"); // block4

        // block2
        addInstruction("SWAP f");
        addInstruction("SUB c");
        addInstruction("SWAP f");

        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("INC a");
        addInstruction("SWAP d");

        // block3
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB b");
        addInstruction("JZERO 14");
        addInstruction("JNEG 13"); // end
        addInstruction("SWAP c");
        addInstruction("SHIFT h");
        addInstruction("SWAP c");

        // block4
        addInstruction("RESET a");
        addInstruction("ADD c");
        addInstruction("SUB f"); // v2 - v1 <=0
        addInstruction("JZERO -18"); // block2
        addInstruction("JNEG -19"); // block2
        addInstruction("SWAP d");
        addInstruction("SHIFT e");
        addInstruction("SWAP d");
        addInstruction("JUMP -16"); // block3

        // if var1 negative and var2 positive = var2 - result (positive) (1)
        // if var1 positive and var2 negative = -(var2-result) (2)
        // if both negative result is just var1 % var2 but negative (3)
        // g == 2 both neg (3)
        // g == -1 var1 < 0, var2 > 0 (1)
        // g == 1 var1 > 0, var2 < 0 (2)
        long long int offset2 = addInstruction("RESET a");
        addInstruction("ADD g");
        addInstruction("JZERO 15"); // don't negate, to end (swap f)
        addInstruction("DEC a"); // g==2-> g==1, g==1->g==0, g==-1->g==-2
        addInstruction("JPOS 10"); // g==1 so previously g == 2, both neg
        addInstruction("SWAP g"); // is g shifted
        // var2-result, var2 in c g == -2
        addInstruction("SWAP b"); // var2 in a
        addInstruction("SUB f"); // var2-result
        addInstruction("SWAP f"); // result
        addInstruction("SWAP g");
        // if var2 is neg, g == 0, negate the res
        addInstruction("JNEG 7"); // to end (with swapping f)
        // var2 < 0, negate it
        addInstruction("RESET a");
        addInstruction("SUB f");
        addInstruction("JUMP 7"); // jump to end (without swapping f)
        // both negative, negate res
        addInstruction("RESET a");
        addInstruction("SUB f");
        addInstruction("JUMP 2");
        addInstruction("SWAP f");
        addInstruction("JUMP 2");
        long long int offset3 = addInstruction("RESET a");
        changeInstruction(offset1, "JZERO " + std::to_string(offset3 - offset1));

        return true;
    }
    return false;
}
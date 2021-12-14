#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "../inc/IOOperations.hpp"

IOOperations::IOOperations(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Invalid number of arguments. Usage kompilator <input file name> <output file name>." << std::endl;
        exit(1);
    }
    // correct number of arguments
    this->inFile = argv[1];
    this->outFile = argv[2];
}

FILE* IOOperations::readCode() {
    FILE *f = fopen(this->inFile.c_str(), "r");

    if(!f) {
        std::cerr << "Cannot open file: " << this->inFile << std::endl;
        return nullptr;
    }
    return f;
}

int IOOperations:: writeCode(std::vector<std::string> translatedCode) {
    std::ofstream out(outFile);

    std::vector<std::string>::iterator it;
    for(it = translatedCode.begin(); it != translatedCode.end(); it++) {
        out << *it << std::endl;
    }

    out.close();
    return 0;
}
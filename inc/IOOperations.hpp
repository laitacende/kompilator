#ifndef KOMPILATOR_IOOPERATIONS_HPP
#define KOMPILATOR_IOOPERATIONS_HPP
// class to handle reading code from file and writing translated code to a file
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

class IOOperations {
public:
    IOOperations(int argc, char** argv);
    FILE* readCode();
    int writeCode(std::vector<std::string> translatedCode);
private:
    std::string inFile;
    std::string outFile;
};
#endif //KOMPILATOR_IOOPERATIONS_HPP

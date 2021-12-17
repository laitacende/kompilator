#ifndef KOMPILATOR_COND_HPP
#define KOMPILATOR_COND_HPP

#include <string>

class Cond {
public:
    long long int index;
    std::string type;
    long long int firstIndex;
    Cond(long long int ind, std::string name);
    ~Cond() {};
    Cond() {};
};

#endif //KOMPILATOR_COND_HPP

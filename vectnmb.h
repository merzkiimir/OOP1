#pragma once
#include <vector>
#include "number.h"

class vectorNumbers {
private:
    std::vector<Number> numbers;
public:
    vectorNumbers(){};
    ~vectorNumbers();
    void add_number(const Number& num);
    void remove_number();
    const std::vector<Number>& get_all() const;
    void printAll() const;
};

vectorNumbers make_vect();
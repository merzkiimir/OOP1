#include "vectnmb.h"
#include "iomanip"
#include "iostream"
#include <sstream>
using namespace std;

vectorNumbers::~vectorNumbers(){}

void vectorNumbers::add_number(const Number& num){
    numbers.push_back(num);
}

void vectorNumbers::remove_number(){
    size_t index;
    cout << "Select index: " << endl;
    cin >> index;
    cin.ignore();
    if (index - 1 < numbers.size()){
        numbers.erase(numbers.begin() + index - 1);
    } else {
        cout << "invalid index" << endl;
    }
}

const std::vector<Number>& vectorNumbers::get_all() const{
    return numbers;
}

void vectorNumbers::printAll() const {
    if (numbers.empty()) {
        cout << "No phone numbers available." << endl;
        return;
    }

    for (size_t i = 0; i < numbers.size(); ++i) {
        cout << left 
                  << setw(15) << numbers[i].get_type()
                  << setw(15) << numbers[i].get_val() << endl;
    }
}

vectorNumbers make_vect(){
    vectorNumbers vct;
    vct.add_number(make_number());
    return vct;
}


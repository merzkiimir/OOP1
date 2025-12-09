#include <iostream>
#include <regex>
#include "number.h"
#include "smt.h"


using namespace std;

Number::Number(const std::string& val, const std::string& type = ""): val(val), type(type) {}

Number::~Number(){};

void Number::set_val(){
    std::string ival;
    static const regex pattern(
        R"(^(\+7|8)\(?\d{3}\)?\d{3}\-?\d{2}\-?\d{2}$)"
    );
    do{
    cout << "input number: ";
    getline(cin, ival);
    ival = space(ival);
    }while (!regex_match(ival, pattern));
    
    this->val = ival;
}

void Number::set_typ(){
    cout << "input type: ";
    getline(cin, type);
    this->type = type;
}

std::string Number::get_val() const{
    return val;
}

std::string Number::get_type() const{
    return type;
}

Number make_number(){
    Number n;
    n.set_val();
    n.set_typ();
    return n;
}
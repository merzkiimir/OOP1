#pragma once
#include "string"

class Number {
    private:
    std::string val;
    std::string type;

    

    public:
    Number(){};
    Number(const std::string& val, const std::string& type);
    ~Number();

    void set_val();
    void set_typ();

    std::string get_val() const;
    std::string get_type() const;

};

Number make_number();

void change_value();
void change_type();
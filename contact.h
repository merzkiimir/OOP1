#pragma once
#include "string"
#include "vector"
#include "vectnmb.h"

class Contact {
private:
	std::string Name;
    std::string surname;
    std::string Name3;
    std::string address;
    std::string date;
    std::string email;
    vectorNumbers numbers;

    

    public:
    Contact();
    Contact(
        const std::string& Name, 
        const std::string& surname, 
        const std::string& Name3, 
        const std::string& address, 
        const std::string& date, 
        const std::string& email,
        vectorNumbers numbers
    );
    Contact(const Contact &oth);
    ~Contact();
    

    void set_name();
    void set_surname();
    void set_name3();
    void set_address();
    void set_date();
    void set_email();
    void set_numbers();

    void edit_numbers();

    void edit_contact();

    std::string get_name() const;
    std::string get_surname() const;
    std::string get_name3() const;
    std::string get_address() const;
    std::string get_date() const;
    std::string get_email() const;
    const vectorNumbers& get_numbers() const;

};

Contact make_contact();

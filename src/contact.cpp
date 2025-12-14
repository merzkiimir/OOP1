#include "contact.h"
#include "iostream"
#include "smt.h"
#include <regex>
using namespace std;

Contact::~Contact(){}

Contact::Contact()
    : Name(""),
      surname(""),
      Name3(""),
      address(""),
      date(""),
      email(""),
      numbers(),
      qtPhones()
{}

Contact::Contact(
        const string& Name,
        const string& surname,
        const string& Name3,
        const string& address,
        const string& date,
        const string& email,
        vectorNumbers numbers
    )
    : Name(Name),
      surname(surname),
      Name3(Name3),
      address(address),
      date(date),
      email(email),
      numbers(numbers),
      qtPhones()
{}

Contact::Contact(const Contact &oth)
    : Name(oth.Name),
      surname(oth.surname),
      Name3(oth.Name3),
      address(oth.address),
      date(oth.date),
      email(oth.email),
      numbers(oth.numbers),
      qtPhones(oth.qtPhones)
{}

void Contact::set_name(){
    std::string nm;
    do{
        cout << "input name: ";
        getline(cin, nm);
        nm = space(nm);
    } while (vld(nm));
    this->Name = nm;
}

void Contact::set_surname(){
    std::string snm;
    do{
        cout << "input surname: ";
        getline(cin, snm);
        snm = space(snm);
    } while (vld(snm));
    this->surname = snm;
}

void Contact::set_name3(){
    std::string nm3;
    do{
        cout << "input otchestvo: ";
        getline(cin, nm3);
        nm3 = space(nm3);
    } while (vld(nm3));
    this->Name3 = nm3;
}

void Contact::set_address(){
    std::string ddr;
    cout << "input address: ";
    getline(cin, ddr);
    ddr = space(ddr);
    this->address = ddr;
}

void Contact::set_email(){
    std::string em;
    static const regex pattern(
        R"(^[A-Za-z][A-Za-z\d]*[@][A-Za-z][A-Za-z\d]*[.][A-Za-z]*$)"
    );
    do{
        cout << "input email: ";
        getline(cin, em);
        em = space(em);
    } while (!regex_match(em, pattern));
    this->email = em;
}


void Contact::set_date() {
    static const regex pattern(
        R"(^(\d{1,2})\.(\d{1,2})\.(\d{4})$)"
    );
    string input;

    while (true) {
        cout << "Enter birth date (DD.MM.YYYY): ";
        getline(cin, input);

        std::smatch match;
        if (!regex_match(input, match, pattern)) {
            cout << "Invalid format! Use DD.MM.YYYY" << endl;
            continue;
        }

        int day = stoi(match[1].str());
        int month = stoi(match[2].str());
        int year = stoi(match[3].str());

        if (!isValidDate(day, month, year)) {
            cout << "Invalid date! Wrong day/month combination." << endl;
            continue;
        }

        if (!isBeforeToday(day, month, year)) {
            cout << "Birth date must be before today." << endl;
            continue;
        }

        this->date = input;
        break;
    }
}

void Contact::edit_numbers(){
    int choice;

    do {
        cout << "\n=== Edit numbers Menu ===" << endl;
        cout << "1. add number" << endl;
        cout << "2. show numbers" << endl; 
        cout << "3. remove {index} number" << endl;        
        cout << "0. Exit Editing" << endl;
        cout << "Select option: " << endl;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                this->numbers.add_number(make_number());
                break;
            case 2:
                cout << "Phone numbers:" << endl;
                this->numbers.printAll();
                break;
            case 3:
                this->numbers.remove_number();
                break;
            case 0:
                cout << "Exiting number edit menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice != 0);
}


void Contact::set_numbers(){
    this->numbers = make_vect();
}

void Contact::edit_contact(){
    int choice;

    do {
        cout << "\n=== Edit Contact Menu ===" << endl;
        cout << "1. Edit Name" << endl;
        cout << "2. Edit Surname" << endl;
        cout << "3. Edit Patronymic (Otchestvo)" << endl;
        cout << "4. Edit Address" << endl;
        cout << "5. Edit Birth Date" << endl;
        cout << "6. Edit Email" << endl;
        cout << "7. Edit Phone Numbers" << endl;
        cout << "0. Exit Editing" << endl;
        cout << "Select option: " << endl;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                this->set_name();
                break;
            case 2:
                this->set_surname();
                break;
            case 3:
                this->set_name3();
                break;
            case 4:
                this->set_address();
                break;
            case 5:
                this->set_date();
                break;
            case 6:
                this->set_email();
                break;
            case 7:
                this->edit_numbers();
                break;
            case 0:
                cout << "Exiting edit menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice != 0);
}

std::string Contact::get_name() const{
    return Name;
}

std::string Contact::get_surname() const{
    return surname;
}

std::string Contact::get_name3() const{
    return Name3;
}

std::string Contact::get_date() const{
    return date;
}

std::string Contact::get_email() const{
    return email;
}

std::string Contact::get_address() const{
    return address;
}

const vectorNumbers& Contact::get_numbers() const{
    return numbers;
}





Contact make_contact(){
    Contact c;
    cout << "required data: name, surname, number, email" << endl;
    c.set_name();
    c.set_surname();
    c.set_email();
    c.set_numbers();
    return c;
}



void Contact::set_qt_phones(const std::vector<std::pair<std::string, std::string>>& phones)
{
    qtPhones = phones;
}

const std::vector<std::pair<std::string, std::string>>& Contact::get_qt_phones() const
{
    return qtPhones;
}

std::string Contact::get_first_qt_phone() const
{
    if (qtPhones.empty())
        return {};
    return qtPhones.front().first;
}

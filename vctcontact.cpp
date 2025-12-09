#include "vctcontact.h"
#include "string"
#include "iostream"
#include "iomanip"
#include "smt.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

vectorContact::~vectorContact(){}

void vectorContact::add_contact(const Contact& ct){
    contact.push_back(ct);
}

void vectorContact::remove_contact(size_t index){
    if (index - 1 < contact.size()){
        contact.erase(contact.begin() + index - 1);
    }
}

void vectorContact::edit_contact(){
    size_t index;
    cout << "Select index: " << endl;
    cin >> index;
    cin.ignore();
    if (index - 1 < contact.size()){
        contact[index - 1].edit_contact();
    } else {
        cout << "invalid index" << endl;
    }
}

std::vector<Contact>& vectorContact::get_all() {
    return contact;
}

const std::vector<Contact>& vectorContact::get_all() const {
    return contact;
}

void vectorContact::printAll() const {
    if (contact.empty()) {
        cout << "No contacts available." << endl;
        return;
    }

    cout << left 
              << setw(4)  << "No"
              << setw(15) << "Name"
              << setw(15) << "Surname"
              << setw(15) << "Patronymic"
              << setw(15) << "Address"
              << setw(15) << "data"
              << setw(25) << "Email" << endl;

    cout << string(100, '-') << endl;

    for (size_t i = 0; i < contact.size(); ++i) {
        cout << left 
                  << setw(4)  << i + 1
                  << setw(15) << contact[i].get_name()
                  << setw(15) << contact[i].get_surname()
                  << setw(15) << contact[i].get_name3()
                  << setw(15) << contact[i].get_address()
                  << setw(15) << contact[i].get_date()
                  << setw(25) << contact[i].get_email();
        cout << endl;
        cout << "\nPhone Numbers:" << endl;
        cout << left 
              << setw(15) << "type"
              << setw(15) << "number" << endl;
        contact[i].get_numbers().printAll();

        cout << endl;
        cout << string(100, '-') << endl;
    }
}

void vectorContact::savetofile(const std::string& filename){
    std::ofstream out(filename, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Error: cannot open file " << filename << " for writing.\n";
        return;
    }

    for (const auto& c : contact) {
        out << c.get_name() << ';'
            << c.get_surname() << ';'
            << c.get_name3() << ';'
            << c.get_address() << ';'
            << c.get_date() << ';'
            << c.get_email() << ';';

        const auto& nums = c.get_numbers().get_all();
        for (size_t i = 0; i < nums.size(); ++i) {
            out << nums[i].get_val() << ',' << nums[i].get_type();
            if (i + 1 < nums.size()) out << '|';
        }
        out << '\n';
    }

    out.close();
    std::cout << "Contacts saved successfully to " << filename << std::endl;
}

void vectorContact::loadfromfile(const string& filename) {
    ifstream in(filename);
    if (!in) {
        ofstream create(filename); 
        cout << "File not found. Created empty file: " << filename << endl;
        return;
    }
    contact.clear();
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string name, surname, name3, address, date, email, numbers_str;
        getline(ss, name, ';');
        getline(ss, surname, ';');
        getline(ss, name3, ';');
        getline(ss, address, ';');
        getline(ss, date, ';');
        getline(ss, email, ';');
        getline(ss, numbers_str);

        vectorNumbers nums;
        stringstream ns(numbers_str);
        string n;
        while (getline(ns, n, '|')) {
            if (n.empty()) continue;
            auto pos = n.find(',');
            string val = n.substr(0, pos);
            string type = n.substr(pos + 1);
            nums.add_number(Number(val, type));
        }

        contact.push_back(Contact(name, surname, name3, address, date, email, nums));
    }
}



vectorContact make_vect_cont(){
    vectorContact vct;
    vct.add_contact(make_contact());
    return vct;
}

vectorContact sorted_vect(const vectorContact &vct1, std::string& param){
    vectorContact sorted;
    const auto& all = vct1.get_all();

    vector<pair<string, size_t>> key_index;

    for(size_t i = 0; i < all.size(); ++i){
        string key;

        if (param == "name") key = space(all[i].get_name());
        else if (param == "surname") key = space(all[i].get_surname());
        else if (param == "patronymic") key = space(all[i].get_name3());
        else if (param == "email") key = space(all[i].get_email());
        else if (param == "address") key = space(all[i].get_address());
        else if (param == "date") key = space(all[i].get_date());
        
        key_index.push_back({ key, i });
    }

    sort(begin(key_index), end(key_index), compare);

    for(const auto& para: key_index)
    {
        sorted.add_contact(all[para.second]);
    }
    return sorted;
}

vectorContact find_cntct(const vectorContact &vct1, std::string& param) {
    vectorContact result;
    const auto& all = vct1.get_all();
    
    for (char &c : param) {
        if (c == ',') c = ' ';
    }

    vector<string> input;
    {
        stringstream ss(param);
        string field;
        while (ss >> field) {
            input.push_back(field);
        }
    }

    static const vector<string> allowed = {
        "name", "surname", "patronymic", "email", "address", "date"
    };

    vector<string> params;

    for (const auto& f : input) {
        if (find(allowed.begin(), allowed.end(), f) == allowed.end()) {
            cout << "Unknown field: " << f << endl;
            return vectorContact();
        }
        params.push_back(f);
    }

    unordered_map<string, string> query;

    for (const auto& p : params) {
        cout << "input " << p << ": ";
        string value;
        getline(cin, value);
        query[p] = space(value);
    }

    for (size_t i = 0; i < all.size(); ++i) {
        const Contact& c = all[i];
        bool match = true;

        for (const auto& kv : query) {
            const string& key = kv.first;
            const string& val = kv.second;
            if (val.empty()) continue;

            string field_value;

            if (key == "name") field_value = c.get_name();
            else if (key == "surname") field_value = c.get_surname();
            else if (key == "patronymic") field_value = c.get_name3();
            else if (key == "email") field_value = c.get_email();
            else if (key == "address") field_value = c.get_address();
            else if (key == "date") field_value = c.get_date();

            field_value = space(field_value);

            if (field_value != val) {
                match = false;
                break;
            }
        }

        if (match) {
            result.add_contact(c);
        }
    }

    return result;
}
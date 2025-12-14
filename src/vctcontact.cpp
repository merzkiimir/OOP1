#include "vctcontact.h"
#include "string"
#include "iostream"
#include "iomanip"
#include "smt.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>

#include <QFile>
#include <QTextStream>
#include <QStringList>

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

void vectorContact::savetofile(const std::string& filename)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: cannot open file " << filename << " for writing.\n";
        return;
    }

    QTextStream out(&file);

    for (const auto& c : contact) {
        out << QString::fromStdString(c.get_name())    << ';'
            << QString::fromStdString(c.get_surname()) << ';'
            << QString::fromStdString(c.get_name3())   << ';'
            << QString::fromStdString(c.get_address()) << ';'
            << QString::fromStdString(c.get_date())    << ';'
            << QString::fromStdString(c.get_email())   << ';';

        const auto &phones = c.get_qt_phones();
        for (size_t i = 0; i < phones.size(); ++i) {
            // first = номер, second = тип
            out << QString::fromStdString(phones[i].first)  << ','
                << QString::fromStdString(phones[i].second);
            if (i + 1 < phones.size())
                out << '|';
        }

        out << '\n';
    }
}

void vectorContact::loadfromfile(const std::string& filename)
{
    QFile file(QString::fromStdString(filename));

    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
            std::cout << "File not found. Created empty file: " << filename << std::endl;
        } else {
            std::cerr << "Error: cannot create file " << filename << std::endl;
        }
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: cannot open file " << filename << " for reading.\n";
        return;
    }

    contact.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;

        QStringList parts = line.split(';');
        if (parts.size() < 7)
            continue;

        QString name        = parts[0];
        QString surname     = parts[1];
        QString name3       = parts[2];
        QString address     = parts[3];
        QString date        = parts[4];
        QString email       = parts[5];
        QString numbers_str = parts[6];

        vectorNumbers nums;
        std::vector<std::pair<std::string, std::string>> qtPhones;

        const auto numberItems = numbers_str.split('|', Qt::SkipEmptyParts);
        for (const QString &item : numberItems) {
            int pos = item.indexOf(',');
            if (pos <= 0) continue;

            QString val  = item.left(pos);
            QString type = item.mid(pos + 1);

            std::string sval  = val.toStdString();
            std::string stype = type.toStdString();

            nums.add_number(Number(sval, stype));
            qtPhones.emplace_back(sval, stype);
        }

        Contact c(
            name.toStdString(),
            surname.toStdString(),
            name3.toStdString(),
            address.toStdString(),
            date.toStdString(),
            email.toStdString(),
            nums
            );
        c.set_qt_phones(qtPhones);

        contact.push_back(c);
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

    auto compare = [](const pair<string, size_t>& left, const pair<string, size_t>& right){
        if (left.first == "" && right.first == ""){
            return left.second < right.second;
        } else if (left.first == ""){
            return false;
        } else if (right.first == ""){
            return true;
        } else {
            return left.first < right.first;
        }
    };

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



vectorContact find_cntct_gui(const vectorContact &vct1,
                             const std::unordered_map<std::string, std::string> &query)
{
    vectorContact result;
    const auto &all = vct1.get_all();

    for (size_t i = 0; i < all.size(); ++i) {
        const Contact &c = all[i];
        bool match = true;

        for (const auto &kv : query) {
            const string &key = kv.first;
            const string &val = space(kv.second);
            if (val.empty())
                continue;

            string field_value;

            if (key == "name")       field_value = c.get_name();
            else if (key == "surname")    field_value = c.get_surname();
            else if (key == "patronymic") field_value = c.get_name3();
            else if (key == "email")      field_value = c.get_email();
            else if (key == "address")    field_value = c.get_address();
            else if (key == "date")       field_value = c.get_date();
            else
                continue;

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

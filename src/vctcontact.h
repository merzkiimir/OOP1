#pragma once
#include "contact.h"
#include <vector>
#include <unordered_map>

class vectorContact {
private:
    std::vector<Contact> contact;
public:
    vectorContact(){};
    ~vectorContact();
    void add_contact(const Contact& ct);
    void remove_contact(size_t index);
    void edit_contact();
    void printAll() const;
    std::vector<Contact>& get_all();
    const std::vector<Contact>& get_all() const;
    
    void savetofile(const std::string& filename);
    void loadfromfile(const std::string& filename);

};

vectorContact make_vect_cont();
vectorContact sorted_vect(const vectorContact &vct1, std::string& param);
vectorContact find_cntct(const vectorContact &vct1, std::string& param);
vectorContact find_cntct_gui(const vectorContact &vct1, const std::unordered_map<std::string, std::string> &query);
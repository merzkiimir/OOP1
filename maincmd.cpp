#include "vctcontact.h"
#include "iostream"
using namespace std;

int main(){
    int choice1, index;
    string filename, param, ps;
    vectorContact ct1, sorted_ct, find_ct;
    do {
        cout << "\n=== Menu ===" << endl;
        cout << "1. load contacts from file" << endl;
        cout << "2. make new vector" << endl;
        cin >> choice1;
        cin.ignore();
        switch (choice1) {
            case 1:
                cout << "input filename: " << endl;
                getline(cin, filename);
                ct1.loadfromfile(filename);
                break;
            case 2:
                cout << "make first contact" << endl;
                ct1 = make_vect_cont();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice1 != 1 && choice1 != 2);

    int choice;

    do {
        cout << "\n=== Menu ===" << endl;
        cout << "1. add contact" << endl;
        cout << "2. show contacts" << endl;
        cout << "3. show sorted by parametr contacts" << endl;
        cout << "4. find contact" << endl;
        cout << "5. edit {index} contact" << endl;
        cout << "6. remove {index} contact" << endl;
        cout << "7. save contacts" << endl;
        cout << "0. Exit" << endl;
        cout << "Select option: " << endl;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                ct1.add_contact(make_contact());
                break;
            case 2:
                ct1.printAll();
                break;
            case 3:
                cout << "input sort parametr: " << endl;
                getline(cin, param);
                sorted_ct = sorted_vect(ct1, param);
                sorted_ct.printAll();
                break;
            case 4:
                cout << "input parametres (allowed: name, surname, patronymic, email, address, date):  " << endl;
                getline(cin, ps);
                find_ct = find_cntct(ct1, ps);
                find_ct.printAll();
                break;
            case 5:
                ct1.edit_contact();
                break;
            case 6:
                cout << "input index" << endl;
                cin >> index;
                cin.ignore();
                ct1.remove_contact(index);
                break;
            case 7:
                cout << "input filename: " << endl;
                getline(cin, filename);
                cout << "Saving contacts..." << endl;
                cout << "Contacts in memory: " << ct1.get_all().size() << endl;
                ct1.savetofile(filename);
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice != 0);
}
#include "string"
#include "iostream"
#include <regex>
#include <cctype>
using namespace std;

std::string space(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    std::string trimmed = str.substr(start, end - start);


    std::string result;
    result.reserve(trimmed.size());
    bool prevSpace = false;

    for (unsigned char ch : trimmed) {
        if (std::isspace(ch)) {
            if (!prevSpace) {
                result.push_back(' ');
                prevSpace = true;
            }
        } else {
            result.push_back(static_cast<char>(ch));
            prevSpace = false;
        }
    }

    return result;
}
bool vld(const string& strk){
    string str = space(strk);
    static const regex pattern(
        R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё\d\- ]*[A-Za-zА-Яа-яЁё\d]$)"
    );

    if (str.empty()) {
        std::cout << "Empty input. Try again.\n";
        return true;
    }

    if (str.front() == '-' || str.back() == '-') {
            cout << "Name cannot start or end with '-'. Try again." << endl;
            return true;

    }

    if (!regex_match(str, pattern)) {
        cout << "Invalid format. Use only letters, digits, hyphen, and space." << endl;
        return true;
    }

    return false;
}



bool isBeforeToday(int d, int m, int y) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int cy = now->tm_year + 1900, cm = now->tm_mon + 1, cd = now->tm_mday;
    if (y < cy) return true;
    if (y == cy && m < cm) return true;
    if (y == cy && m == cm && d < cd) return true;
    return false;
}

bool isLeapYear(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

bool isValidDate(int d, int m, int y) {
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (isLeapYear(y)) daysInMonth[1] = 29;
    return (m >= 1 && m <= 12 && d >= 1 && d <= daysInMonth[m-1]);
}

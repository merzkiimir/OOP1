#pragma once
#include "string"
using namespace std;

string space(const string& str);
bool vld(const string& strk);
bool isBeforeToday(int d, int m, int y);
bool isValidDate(int d, int m, int y);
bool isLeapYear(int y);
bool compare(const pair<string, size_t>& left, const pair<string, size_t>& right);
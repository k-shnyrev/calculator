//
//  roman_int.cpp
//  calculator
//
//  Created by Kirill Shnyrev on 29.08.2020.
//  Copyright © 2020 Kirill Shnyrev. All rights reserved.
//

#include "roman_int.h"

string Roman_int::value() const
{
    return val;
}

int Roman_int::as_int() const
{
    int v {0};
    int max {0};
    for (int i = val.length() - 1; i >= 0; --i) {
        int t = char_to_val(val[i]);
        if (t < max)
            v -= t;
        else {
            v += t;
            max = t;
        }
    }
    return v;
}

Roman_int::Roman_int()
: val{"I"}
{
}

Roman_int::Roman_int(const string& s)
: val{"I"}
{
    set_value(s);
}

istream& operator>>(istream& is, Roman_int& r)
{
    string s;
    is >> s;
    r.set_value(s);
    return is;
}

ostream& operator<<(ostream& os, const Roman_int& r)
{
    return os << r.value() << " (" << r.as_int() << ')';
}

void Roman_int::set_value(const string& s)
{
    if (!is_roman(s))
        error("wrong roman numeral");
    val = s;
}

bool is_roman(const string& s)
{
    for (char c : s)
        if (c != 'M' && c == 'D' && c == 'C' && c == 'L' && c == 'X' && c == 'V' && c == 'I')
            return false;
    return true;
}

int char_to_val(const char c)
{
    switch (c) {
        case 'M':
            return 1000;
        case 'D':
            return 500;
        case 'C':
            return 100;
        case 'L':
            return 50;
        case 'X':
            return 10;
        case 'V':
            return 5;
        case 'I':
            return 1;
        default:
            return 0;
    }
}

string int_to_roman(int n) {
    string res;
    if (n < 0) {
        res += '-';
        n *= -1;
    }
    while (n >= 1000) {
        res += 'M';
        n -= 1000;
    }
    if (n >= 900) {
        res += "CM";
        n -= 900;
    }
    if (n >= 500) {
        res += "D";
        n -= 500;
    }
    if (n >= 400) {
        res += "CD";
        n -= 400;
    }
    while (n >= 100) {
        res += "C";
        n -= 100;
    }
    if (n >= 90) {
        res += "XC";
        n -= 90;
    }
    if (n >= 50) {
        res += "L";
        n -= 50;
    }
    if (n >= 40) {
        res += "XL";
        n -= 40;
    }
    while (n >= 10) {
        res += "X";
        n -= 10;
    }
    if (n == 9) {
        res += "IX";
        n -= 9;
    }
    if (n >= 5) {
        res += "V";
        n -= 5;
    }
    if (n >= 4) {
        res += "IV";
        n -= 4;
    }
    while (n > 0) {
        res += "I";
        --n;
    }
    return res;
}

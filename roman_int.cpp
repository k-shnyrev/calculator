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

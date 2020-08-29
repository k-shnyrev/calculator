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
    int t {0};
    for (char c : val) {
        switch (c) {
            case 'M':
                if (t > 1000) {
                    v += t;
                    t = 1000;
                }
                else if (t < 1000)
                    t = 1000 - t;
                else
                    t += 1000;
                break;
            case 'D':
                if (t > 500) {
                    v += t;
                    t = 500;
                }
                else if (t < 500)
                    t = 500 - t;
                else
                    t += 500;
                break;
            case 'C':
                if (t > 100) {
                    v += t;
                    t = 100;
                }
                else if (t < 100)
                    t = 100 - t;
                else
                    t += 100;
                break;
            case 'L':
                if (t > 50) {
                    v += t;
                    t = 50;
                }
                else if (t < 50)
                    t = 50 - t;
                else
                    t += 50;
                break;
            case 'X':
                if (t > 10) {
                    v += t;
                    t = 10;
                }
                else if (t < 10)
                    t = 10 - t;
                else
                    t += 10;
                break;
            case 'I':
                if (t > 1) {
                    v += t;
                    t = 1;
                }
                else
                    t += 1;
                break;
            default:
                error("wrong char");
                break;
        }
    }
    v += t;
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

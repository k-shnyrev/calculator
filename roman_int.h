//
//  roman_int.h
//  calculator
//
//  Created by Kirill Shnyrev on 29.08.2020.
//  Copyright © 2020 Kirill Shnyrev. All rights reserved.
//

#include "std_lib_facilities.h"

class Roman_int {
    string val;
public:
    Roman_int(const string& s);
    Roman_int();
    void set_value(const string& s);
    string value() const;
    int as_int() const;
};

istream& operator>>(istream& is, Roman_int& r);
ostream& operator<<(ostream& os, const Roman_int& r);

bool is_roman(const string& s);
int char_to_val(const char c);
string int_to_roman(int n);

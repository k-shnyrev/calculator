#pragma once

#include "std_lib_facilities.h"

struct Variable {
	string name;
	double value;
	bool is_const;
	Variable(string n, double v) :name(n), value(v), is_const(false) { }
	Variable(string n, double v, bool c) :name(n), value(v), is_const(c) { }
};

class Symbol_table {
	vector<Variable> var_table;
public:
	double get(string s);
	double set(string s, double d);
	bool is_declared(string s);
	double define(string var, double val, bool cnst);
};
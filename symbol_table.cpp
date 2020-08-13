#include "symbol_table.h"

double Symbol_table::get(string s)
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) return var_table[i].value;
	error("get_value: undefined name ", s);
}

double Symbol_table::set(string s, double d)
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) {
			if (!var_table[i].is_const) {
				var_table[i].value = d;
				return var_table[i].value;
			}
			error("set: " + s + " is const");
		}
	error("set: undefined name ", s);
}

bool Symbol_table::is_declared(string s)
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) return true;
	return false;
}

double Symbol_table::define(string var, double val, bool cnst)
{
	if (is_declared(var))
		error(var, " declared twice");
	var_table.push_back(Variable(var, val, cnst));
	return val;
}
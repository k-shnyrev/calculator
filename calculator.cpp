/*
	Based on a calculator from "Programming: Principles and Practice Using C++" by B. Stroustrup, 2nd edotion.
	Exercises from Chapters 7 and 8.

	This program implements a basic expression calculator.

	Input from cin *to add: input from any stream*; output to cout.

	The grammar for input is:

	Calculation:
		Instruction
		Print
		Exit
		Help
		Calsulation Instruction
        from x
        to y

	Instruction:
		Declaration
		Const_Declaration
		Expression

	Declaration:
		'let' Name '=' Expression
		'#' Name '=' Expression

	Const_Declaration:
		'const' Name = Expression

	Print:
		';'
		'\n'

	Exit:
		'quit'
		'exit'
		'q'

	Help:
		'h'
		'H'
		'help'

	Expression:
		Term
		Expression + Term
		Expression - Term

	Term:
		Primary
		Term * Primary
		Term / Primary
		Term % Primary

	Primary:
		Number
		( Expression )
		- Primary
		+ Primary
		Name
		sqrt ( Expression )
		pow ( Expression , Expression )
		Name = Expression

	Number:
		floating-point-literal

	Name:
		same-as-var-names

	Input comes from cin through the Token_stream called ts.
*/
#define _USE_MATH_DEFINES

#include "token_stream.h"
#include "symbol_table.h"
#include <cmath>

void print_help()
{
	cout << "--------------------------------------------------------------------------------------------------------\n"
		<< "Use 'help' for help,\n"
		<< "'quit' for quit,\n"
		<< "';' for the end of the command to see the result.\n"
        << "'from x' to read commands from file 'x'\n"
        << "'to y' to write results to file 'y'"
		<< "\n"
		<< "Use 'let' or '#' to make a variable, names can start with letter and consist of letters, digits and '-':\n"
		<< "example: 'let a = 3; # b_5 = a * 3;'.\n"
		<< "Use 'const' to make a constant, for example 'const p = 2'.\n"
		<< "Use 'a = 4' to change the value of already set (!) variable.\n"
		<< "\n"
		<< "You can use usual operations '+', '-', '*', '/', '%' as mod;\n"
		<< "sqrt(x) for square root;\n"
		<< "pow(x, n) to calculate x to the power of n (natural or zero).\n"
		<< "--------------------------------------------------------------------------------------------------------\n";
}

Symbol_table table;

double expression(Token_stream& ts);

double my_pow(double x, int i)
{
	if (i < 0)
		error("negative power");
	double p = 1;
	if (i == 0)
		return p;
	for (int k = 0; k < i; ++k)
		p *= x;
	return p;
}

double primary(Token_stream &ts)
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{
		double d = expression(ts);
		t = ts.get();
		if (t.kind != ')')
			error("')' expected");
		return d;
	}
	case '-':
		return -primary(ts);
	case '+':
		return primary(ts);
	case number:
		return t.value;
	case name:
	{
		if (!table.is_declared(t.name)) error(t.name + " is not defined");
		Token t2 = ts.get();
		if (t2.kind == '=') {
			double var_value = expression(ts);
			return table.set(t.name, var_value);
		}
		else {
			ts.unget(t2);
			return table.get(t.name);
		}
	}
	case sqroot:
		t = ts.get();
		if (t.kind == '(') {
			double d = expression(ts);
			t = ts.get();
			if (t.kind != ')')
				error("'(' expected");
			if (d >= 0)
				return sqrt(d);
			else
				error("sqare root of a negative number");
		}
		else
			error("'(' expected");
	case power:
		t = ts.get();
		if (t.kind == '(') {
			double x = expression(ts);
			t = ts.get();
			if (t.kind != sep)
				error(sep + " expected");
			int i = narrow_cast<int>(expression(ts));
			t = ts.get();
			if (t.kind != ')')
				error("')' expected");
			return my_pow(x, i);
		}
		else
			error("'(' expected");
	default:
		error("primary expected");
	}
    return 0;
}

double term(Token_stream& ts)
{
	double left = primary(ts);
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary(ts);
			break;
		case '/':
		{
			double d = primary(ts);
			if (d == 0) error("/: divide by zero");
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary(ts);
			if (d == 0) error("%: divided by zero");
			left = fmod(left, d);
			break;
		}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression(Token_stream& ts)
{
	double left = term(ts);
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term(ts);
			break;
		case '-':
			left -= term(ts);
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration(Token_stream& ts, bool cnst)
{
	Token t = ts.get();
	if (t.kind != name) {
		ts.unget(t);
		error("name expected in declaration");
	}
	string var_name = t.name;
	Token t2 = ts.get();
	if (t2.kind != '=') {
		ts.unget(t2);
		error("= missing in declaration of ", var_name);
	}
	double d = expression(ts);
	table.define(var_name, d, cnst);
	return d;
}

double statement(Token_stream& ts)
{
	Token t = ts.get();
	switch (t.kind) {
	case let:
		return declaration(ts, false);
	case t_const:
		return declaration(ts, true);
	default:
		ts.unget(t);
		return expression(ts);
	}
}

void clean_up_mess(Token_stream& ts)
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate(Token_stream& ts)
{
	while (true) try {
		ts.get_out_stream() << prompt;
		Token t = ts.get();
		while (t.kind == print || t.kind == newline) { // ignore while ';' or '\n'
			t = ts.get();
		}
		if (t.kind == quit) return;
		if (t.kind == help) print_help();
        if (t.kind == from) {
            string iname;
            ts.get_in_stream() >> iname;
            ifstream ifs {iname};
            if (!ifs)
                error("can't open input file ", iname);
            Token_stream ts_new {ifs};
            calculate(ts_new);
        }
        else if (t.kind == to) {
            string oname;
            ts.get_in_stream() >> oname;
            ofstream ofs {oname};
            if (!ofs)
                error("can't open output file ", oname);
            Token_stream ts_new {ts.get_in_stream(), ofs};
            calculate(ts_new);
        }
		else {
			ts.unget(t);
			ts.get_out_stream() << result << statement(ts) << '\n';
		}
	}
	catch (runtime_error& e) {
		cerr << e.what() << '\n';
		clean_up_mess(ts);
	}
}

int main()
try {
	table.define("pi", M_PI, true);
	table.define("e", M_E, true);
	Token_stream ts;
	calculate(ts);
	return 0;
}
catch (exception& e) {
	cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...) {
	cerr << "Unknown error\n";
	return 2;
}

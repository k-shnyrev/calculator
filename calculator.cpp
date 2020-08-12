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

#include "std_lib_facilities.h"
#include <cmath>

void print_help()
{
	cout << "--------------------------------------------------------------------------------------------------------\n"
		<< "Use 'help' for help,\n"
		<< "'quit' for quit,\n"
		<< "';' for the end of the command to see the result.\n"
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

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), value(0), name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
	istream& st;
public:
	Token_stream() :full(0), buffer(0), st{ cin } { }
	Token_stream(istream& s) :full(0), buffer(0), st{ s } { }
	Token get();
	void unget(Token t) { buffer = t; full = true; }

	void ignore(char c);
};

// Token kinds
const char let = '#';
const char quit = 'q';
const char help = 'h';
const char print = ';';
const char newline = '\n';
const char number = '8';
const char name = 'a';
const char sqroot = 'r';
const char power = 'p';
const char sep = ',';
const char t_const = 'c';

Token Token_stream::get()
{
	if (full) {
		full = false;
		return buffer;
	} 
	char ch;
	st.get(ch);
	while (isspace(ch)) {
		if (ch == '\n')
			return newline;
		st.get(ch);
	}
	switch (ch) {
	case '\n':
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case ',':
	case '#':
		return Token{ ch };
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9': {
		st.unget();
		double val;
		st >> val;
		return Token{ number, val };
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (st.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s += ch; // letter-by-letter reading a word consisting of letters, digits and '_'
			st.unget();
			if (s == "let") return Token{ let };
			if (s == "quit" || s == "exit" || s == "q") return Token{ quit };
			if (s == "help" || s == "h" || s == "H") return Token{ help };
			if (s == "sqrt") return Token{ sqroot };
			if (s == "pow") return Token{ power };
			if (s == "const") return Token{ t_const };
			return Token{ name, s }; // if not a reserved word
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) { // check buffer first
		full = false;
		return;
	}
	full = false;

	char ch;
	while (st.get(ch)) {
		if (ch == newline) ch = print; // print and newline are the same for ignore()
		if (ch == c) return;
	}
}

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
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print || t.kind == newline) { // ignore while ';' or '\n'
			t = ts.get();
		}
		if (t.kind == quit) return;
		if (t.kind == help) print_help();
		else {
			ts.unget(t);
			cout << result << statement(ts) << '\n';
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
	cerr << "exception: " << e.what() << '\n';
	/*char c;
	while (cin >> c && c != ';');*/
	return 1;
}
catch (...) {
	cerr << "exception\n";
	/*char c;
	while (cin >> c && c != ';');*/
	return 2;
}

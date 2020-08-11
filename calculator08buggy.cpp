/*
	�������� �� ������������ �� ����� ������ ����������� "����������������: �������� � �������� � �������������� C++", ���������� � ����� ����� 7.

	��� ��������� ��������� �������� ��������� ������������.

	���� �������������� �� ������ cin; ����� - � ����� cout.

	���������� ��� �����:

	����������:
		����������
		�����
		�����
		���������� ����������

	����������:
		����������
		����������_���������
		���������

	����������:
		let ��� = ���������
		# ��� = ���������

	���������� ���������:
		const ��� = ���������
		
	�����:
		;
	
	�����:
		quit
		exit

	���������:
		����
		��������� + ����
		��������� - ����

	����:
		���������_���������
		���� * ���������_���������
		���� / ���������_���������
		���� % ���������_���������
		
	���������_���������:
		�����
		( ��������� )
		- ���������_���������
		+ ���������_���������
		���
		sqrt ( ��������� )
		pow ( ��������� , ��������� )
		��� = ���������

	�����:
		�������_�_���������_������

	���:
		�������_��_����_����_�_�������������_������������_�_�����

	���� �� ������ cin ����� Token_stream � ������ ts.
*/
#define _USE_MATH_DEFINES

#include "std_lib_facilities.h"
#include <cmath>

struct Token { // ����� ������� �� ����, �������� � (��������) ����� ����������, ����� ������������������ ������ ����� (���������) ��� ����� � ��������� (�����)
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), value(0), name(n) { }
};

class Token_stream { // ����� � ������� ��� ������ �������
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get(); // ��������� ���������� ������ �� ������ �������
	void unget(Token t) { buffer = t; full = true; } // ������� ������ � ����� �������

	void ignore(char c); // ������������ ���� �� ��������� �������, ����������� ��� ��������
};

// �������� ��������, ������������ ���� �������
const char let = '#';
const char quit = 'q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqroot = 'r';
const char power = 'p';
const char sep = ',';
const char t_const = 'c';

Token Token_stream::get() // ��������� ���������� ������ �� ������ �������
{
	if (full) { // ���� � ������ ���� �����, �������� ��� ������
		full = false;
		return buffer;
	} 
	char ch;
	cin >> ch;
	switch (ch) {
	case '(': // ��� ����������
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
	case '.': // ��� �����
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
		cin.unget();
		double val;
		cin >> val;
		return Token{ number, val };
	}
	default: // ��� ���������� ��� ��������� ���� 'let', 'quit', ������ ���������� � �����
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s += ch; // ������ ����������� ����� (�� ����, ���� � '_')
			cin.unget();
			if (s == "let") return Token{ let }; // ��������� ��������� ����
			if (s == "quit" || s == "exit") return Token{ quit };
			if (s == "sqrt") return Token{ sqroot };
			if (s == "pow") return Token{ power };
			if (s == "const") return Token{ t_const };
			return Token{ name, s }; // ���� �� ��������� �����, �� ����������
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c) // ������������ ���� �� ��������� ������� (�), ����������� ��� ��������
{
	if (full && c == buffer.kind) { // ���� ������ c � ������, �� ���������������, ������� �����
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch)
		if (ch == c) return; // ���������������, ����� ����� ������ c
}

struct Variable { // ��� ���������� ������������ �������� � ������
	string name;
	double value;
	bool is_const;
	Variable(string n, double v) :name(n), value(v), is_const(false) { }
	Variable(string n, double v, bool c) :name(n), value(v), is_const(c) { }
};

class Symbol_table { // ������� ���������� � ��������
	vector<Variable> var_table;
public:
	double get(string s); // ��������� �������� ���������� �� �����
	double set(string s, double d); // ������������ �������� ������������ ����������
	bool is_declared(string s); // �������� ������������� ����������
	double define(string var, double val, bool cnst); // ���������� ����� ���������� � ���������� ������� ����������
};

double Symbol_table::get(string s) // ��������� �������� ���������� �� �����
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) return var_table[i].value;
	error("get_value: undefined name ", s);
}

double Symbol_table::set(string s, double d) // ������������ �������� ������������ ����������
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

bool Symbol_table::is_declared(string s) // �������� ������������� ����������
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) return true;
	return false;
}

double Symbol_table::define(string var, double val, bool cnst) // ���������� ����� ���������� � ���������� ������� ����������
{	
	if (is_declared(var))
		error(var, " declared twice");
	var_table.push_back(Variable(var, val, cnst));
	return val;
}

Token_stream ts; // ����� �������

Symbol_table table; // ������� ����������

double expression();

double my_pow(double x, int i) // ���������� x � ������� i
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

double primary() // ������ ���������� ��������� -- �����, ������������� ��������� ���������, ��������� � �������, ������������ ����������, sqrt ( ��������� ), pow ( ��������� , ��������� ), ��� = ���������
{
	Token t = ts.get();
	switch (t.kind) {
	case '(': // ��������� � �������
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')')
			error("'(' expected");
		return d;
	}
	case '-': // ������������� ��������� ���������
		return -primary();
	case '+':
		return primary();
	case number: // �����
		return t.value;
	case name: // ������������ ����������
	{
		Token t2 = ts.get();
		if (t2.kind == '=') {
			double var_value = expression();
			return table.set(t.name, var_value);
		}
		else {
			ts.unget(t2);
			return table.get(t.name);
		}
	}
	case sqroot: // sqrt( ��������� )
		t = ts.get();
		if (t.kind == '(') {
			double d = expression();
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
	case power: // pow ( ��������� , ��������� )
		t = ts.get();
		if (t.kind == '(') {
			double x = expression();
			t = ts.get();
			if (t.kind != sep)
				error(sep + " expected");
			int i = narrow_cast<int>(expression());
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

double term() // ������ ����� -- ��������� ���������, ���� * �����, ���� / �����
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("/: divide by zero");
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary();
			if (d == 0) error("%: divided by zero");
			left = fmod(left, d);
			t = ts.get();
			break;
		}
		default: // ���� ����������
			ts.unget(t);
			return left;
		}
	}
}

double expression() // ������ ��������� -- ����, ��������� + ����, ��������� - ����
{
	double left = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default: // ��������� �����������
			ts.unget(t);
			return left;
		}
	}
}

double declaration(bool cnst) // ���������� ���������� - 'let ��� = ���������', '# ��� = ���������' ��� ��������� - 'const ��� = ���������'
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
	double d = expression();
	table.define(var_name, d, cnst);
	return d;
}

double statement() // ������ ���������� -- ����������, ���������
{
	Token t = ts.get();
	switch (t.kind) {
	case let: // ����������
		return declaration(false);
	case t_const:
		return declaration(true);
	default: // ���������
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess() // ������� ������ -- ������������� ������ �� ������� ������ ���������� ������������
{
	ts.ignore(print);
}

const string prompt = "> "; // ����������� �� �����
const string result = "= "; // ������ ������ ����������

void calculate() // ��������� ���������� -- ����������, �����, �����, ���������� ����������
{
	while (true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get(); // ���������� ��������� �������� ������ ������
		if (t.kind == quit) return; // ����� �� ���������
		ts.unget(t); // ���� �� ����� �� ����� � �� ����� �� ���������, �� ���������� ����� � ����� ����� � ������������ ����������, ������ ���������
		cout << result << statement() << endl;
	}
	catch (runtime_error& e) { // � ������ ������ ������� ��������� � ������� ����� � ������ �������, �� �� ��������� ���������
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()

try {
	table.define("pi", M_PI, true);
	table.define("e", M_E, true);

	calculate();
	return 0;
}
catch (exception& e) { // ��� ��������� ������
	cerr << "exception: " << e.what() << endl;
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

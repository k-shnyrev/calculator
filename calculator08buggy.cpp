/*
	�������� �� ������������ �� ����� ������ ����������� "����������������: �������� � �������� � �������������� C++", ���������� � ����� ����� 7.

	��� ��������� ��������� �������� ��������� ������������.

	���� �������������� �� ������ cin; ����� - � ����� cout.

	���������� ��� �����:

	����������:
		���������
		�����
		�����

	�����:
		;
	
	�����:
		quit

	���������:
		����
		��������� + ����
		��������� - ����

	����:
		���������_���������
		���� * ���������_���������
		���� / ���������_���������
		X ���� % ���������_���������
		
	���������_���������:
		�����
		( ��������� )
		- ���������_���������
		X + ���������_���������

	�����:
		�������_�_���������_������

	���� �� ������ cin ����� Token_stream � ������ ts.
*/
#define _USE_MATH_DEFINES

#include "../../std_lib_facilities.h"
#include <math.h>

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
const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';

Token Token_stream::get() // ��������� ���������� ������ �� ������ �������
{
	if (full) { full = false; return buffer; } // ���� � ������ ���� �����, �������� ��� ������
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
		return Token(ch);
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
		return Token(number, val);
	}
	default: // ��� ���������� ��� ��������� ���� 'let', 'quit'
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch; // ������ ����������� ����� (�� ���� � ����)
			cin.unget();
			if (s == "let") return Token(let); // ��������� ��������� ����
			if (s == "quit") return Token(quit);
			return Token(name, s); // ���� �� ��������� �����, �� ����������
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
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names; // ���������� ������� ����������

double get_value(string s) // ��������� �������� ���������� �� �����
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ", s);
}

void set_value(string s, double d) // ������������ �������� ������������ ����������
{
	for (int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s) // �������� ������������� ����������
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts; // ����� �������

double expression();

double primary() // ������ ���������� ��������� -- �����, ������������� ��������� ���������, ��������� � �������, ������������ ����������
{
	Token t = ts.get();
	switch (t.kind) {
	case '(': // ��������� � �������
	{	double d = expression();
	t = ts.get();
	if (t.kind != ')') error("'(' expected");
	}
	case '-': // ������������� ��������� ���������
		return -primary();
	case number: // �����
		return t.value;
	case name: // ������������ ����������
		return get_value(t.name);
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
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		default: // ���� ����������
			ts.unget(t);
			return left;
		}
	}
}

double expression() // ������ ��������� -- ���� + ����, ��������� + ����, ���� - ����, ��������� - ����
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

double define_name(string var, double val) { // ���������� ����� ���������� � ���������� ������� ����������
	if (is_declared(var))
		error(var, " declared twice");
	names.push_back(Variable(var, val));
	return val;
}

double declaration() // ���������� ���������� - 'let ��� = ���������'
{
	Token t = ts.get();
	if (t.kind != name)
		error("name expected in declaration");
	string var_name = t.name;
	Token t2 = ts.get();
	if (t2.kind != '=')
		error("= missing in declaration of ", var_name);
	double d = expression();
	define_name(var_name, d);
	return d;
}

double statement() // ������ ���������� -- ���������� ��� ���������
{
	Token t = ts.get();
	switch (t.kind) {
	case let: // ����������
		return declaration();
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
	define_name("pi", M_PI);
	define_name("e", M_E);

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

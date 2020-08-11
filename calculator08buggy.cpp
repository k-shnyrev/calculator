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
		���������

	����������:
		let ��� = ���������
		# ��� = ���������
		
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
		sqrt ( ��������� )
		pow ( ��������� , ��������� )

	�����:
		�������_�_���������_������

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
	default: // ��� ���������� ��� ��������� ���� 'let', 'quit'
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch; // ������ ����������� ����� (�� ���� � ����)
			cin.unget();
			if (s == "let") return Token{ let }; // ��������� ��������� ����
			if (s == "quit" || s == "exit") return Token{ quit };
			if (s == "sqrt") return Token{ sqroot };
			if (s == "pow") return Token{ power };
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
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names; // ���������� ������� ����������

double get_value(string s) // ��������� �������� ���������� �� �����
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get_value: undefined name ", s);
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

double primary() // ������ ���������� ��������� -- �����, ������������� ��������� ���������, ��������� � �������, ������������ ����������, sqrt ( ��������� ), pow ( ��������� , ��������� )
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
		return get_value(t.name);
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
				error("',' expected");
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

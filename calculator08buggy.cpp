/*
	Основано на калькуляторе из книги Бьярне Страуструпа "Программирование: принципы и практика с использованием C++", упражнения в конце главы 7.

	Эта программа реализует основные выражения калькулятора.

	Ввод осуществляется из потока cin; вывод - в потом cout.

	Грамматика для ввода:

	Вычисление:
		Инструкция
		Вывод
		Выход
		Вычисление Инструкция

	Инструкция:
		Объявление
		Выражение

	Объявление:
		let Имя = Выражение
		# Имя = Выражение
		
	Вывод:
		;
	
	Выход:
		quit
		exit

	Выражение:
		Терм
		Выражение + Терм
		Выражение - Терм

	Терм:
		Первичное_выражение
		Терм * Первичное_выражение
		Терм / Первичное_выражение
		Терм % Первичное_выражение
		
	Первичное_выражение:
		Число
		( Выражение )
		- Первичное_выражение
		+ Первичное_выражение
		sqrt ( Выражение )
		pow ( Выражение , Выражение )

	Число:
		Литерал_с_плавающей_точкой

	Ввод из потока cin через Token_stream с именем ts.
*/
#define _USE_MATH_DEFINES

#include "std_lib_facilities.h"
#include <cmath>

struct Token { // токен состоит из типа, значения и (возможно) имени переменной, может инициализироваться только типом (операторы) или типом и значением (числа)
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), value(0), name(n) { }
};

class Token_stream { // поток с буфером для чтения токенов
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get(); // получение следующего токена из потока токенов
	void unget(Token t) { buffer = t; full = true; } // возврат токена в поток токенов

	void ignore(char c); // игнорировать ввод до появления символа, переданного как аргумент
};

// описание констант, обозначающих типы токенов
const char let = '#';
const char quit = 'q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqroot = 'r';
const char power = 'p';
const char sep = ',';

Token Token_stream::get() // получение следующего токена из потока токенов
{
	if (full) { // если в буфере есть токен, забираем его оттуда
		full = false;
		return buffer;
	} 
	char ch;
	cin >> ch;
	switch (ch) {
	case '(': // для операторов
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
	case '.': // для чисел
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
	default: // для переменных или служебных слов 'let', 'quit'
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch; // читаем посимвольно слово (из букв и цифр)
			cin.unget();
			if (s == "let") return Token{ let }; // обработка служебных слов
			if (s == "quit" || s == "exit") return Token{ quit };
			if (s == "sqrt") return Token{ sqroot };
			if (s == "pow") return Token{ power };
			return Token{ name, s }; // если не служебное слово, то переменная
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c) // игнорировать ввод до появления символа (с), переданного как аргумент
{
	if (full && c == buffer.kind) { // если символ c в буфере, то останавливаемся, очистив буфер
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch)
		if (ch == c) return; // останавливаемся, когда найдём символ c
}

struct Variable { // так переменные калькулятора хранятся в памяти
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names; // глобальная таблица переменных

double get_value(string s) // получение значения переменной по имени
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get_value: undefined name ", s);
}

void set_value(string s, double d) // установление значения существующей переменной
{
	for (int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s) // проверка существования переменной
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts; // поток токенов

double expression();

double my_pow(double x, int i) // возведение x в степень i
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

double primary() // чтение первичного выражения -- число, отрицательное первичное выражение, выражение в скобках, существующая переменная, sqrt ( Выражение ), pow ( Выражение , Выражение )
{
	Token t = ts.get();
	switch (t.kind) {
	case '(': // выражение в скобках
	{	
		double d = expression();
		t = ts.get();
		if (t.kind != ')')
			error("'(' expected");
		return d;
	}
	case '-': // отрицательное первичное выражение
		return -primary();
	case '+':
		return primary();
	case number: // число
		return t.value;
	case name: // существующая переменная
		return get_value(t.name);
	case sqroot: // sqrt( Выражение )
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
	case power: // pow ( Выражение , Выражение )
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

double term() // чтение терма -- первичное выражение, терм * число, терм / число
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
		default: // терм закончился
			ts.unget(t);
			return left;
		}
	}
}

double expression() // чтение выражения -- терм + терм, выражение + терм, терм - терм, выражение - терм
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
		default: // выражение закончилось
			ts.unget(t);
			return left;
		}
	}
}

double define_name(string var, double val) { // добавление новой переменной в глобальную таблицу переменных
	if (is_declared(var))
		error(var, " declared twice");
	names.push_back(Variable(var, val));
	return val;
}

double declaration() // объявление переменной - 'let имя = выражение'
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

double statement() // чтение инструкции -- объявление или выражение
{
	Token t = ts.get();
	switch (t.kind) {
	case let: // объявление
		return declaration();
	default: // выражение
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess() // очистка мусора -- игнорирование буфера до символа вывода результата включительно
{
	ts.ignore(print);
}

const string prompt = "> "; // приглашение ко вводу
const string result = "= "; // начало вывода результата

void calculate() // обработка вычислений -- инструкция, вывод, выход, вычисление инструкция
{
	while (true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get(); // пропускаем несколько символов вывода подряд
		if (t.kind == quit) return; // выход из программы
		ts.unget(t); // если не вывод на экран и не выход из программы, то возвращаем токен в поток ввода и обрабатываем инструкцию, выводя результат
		cout << result << statement() << endl;
	}
	catch (runtime_error& e) { // в случае ошибки выводим сообщение и очищаем мусор в потоке токенов, но не завершаем программу
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
catch (exception& e) { // все остальные ошибки
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

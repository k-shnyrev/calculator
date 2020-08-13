#pragma once

#include "std_lib_facilities.h"

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), value(0), name(n) { }
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
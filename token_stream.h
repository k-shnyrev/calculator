#pragma once

#include "token.h"

class Token_stream {
	bool full;
	Token buffer;
	istream& st;
public:
	Token_stream() :full(0), buffer(0), st{ cin } { }
	Token_stream(istream& s) :full(0), buffer(0), st{ s } { }
	Token get();
	void unget(Token t);
	void ignore(char c);
};
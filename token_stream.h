#pragma once

#include "token.h"

class Token_stream {
	bool full;
	Token buffer;
	istream& st;
    ostream& os;
public:
	Token_stream() :full(0), buffer(0), st{ cin }, os {cout} { }
	Token_stream(istream& s) :full(0), buffer(0), st{ s }, os {cout} { }
    Token_stream(istream& s, ostream& s2) :full(0), buffer(0), st{ s }, os {s2} { }
	Token get();
	void unget(Token t);
	void ignore(char c);
    istream& get_in_stream() const;
    ostream& get_out_stream() const;
};

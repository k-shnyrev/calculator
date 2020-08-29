#include "token_stream.h"

void Token_stream::unget(Token t)
{
	buffer = t;
	full = true;
}

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
    if (st.eof())
        return Token{quit};
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
            if (s == "from") return Token{from};
            if (s == "to") return Token{to};
			return Token{ name, s }; // if not a reserved word
		}
		error("Bad token");
	}
    return 0;
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

istream& Token_stream::get_in_stream() const
{
    return st;
}

ostream& Token_stream::get_out_stream() const
{
    return os;
}

#ifndef UNDEFINEDAUTOMATON_H
#define UNDEFINEDAUTOMATON_H

#include <string>

#include "Automaton.h"
#include "Token.h"


class UndefinedAutomaton: public Automaton {
private:
	std::string rejectedString;
public:
	UndefinedAutomaton() {};
	~UndefinedAutomaton() {};
	int Start (std::string input) {
		rejectedString = "";
		inputRead = 1;
		rejectedString = input.substr(0, 1);
		return inputRead;
	};
	int NewLinesRead() {
		newLines = 0;
		return newLines;
	};
	Token* CreateToken() {
		Token* token = new Token();
		token->SetString(rejectedString);
		token->SetType(UNDEFINED);
		return token;
	};
};
#endif

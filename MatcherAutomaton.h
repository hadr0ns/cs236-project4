#ifndef MATCHERAUTOMATON_H
#define MATCHERAUTOMATON_H


#include "Automaton.h"

class MatcherAutomaton: public Automaton {
private:
	int newLines;
public:
	MatcherAutomaton() {};
	~MatcherAutomaton() {};
	int Start(std::string input) {
		return 0;
	};
	int NewLinesRead() {
		newLines = 0;

		return newLines;
	};
	Token* CreateToken() {
		Token* token = new Token();
		return token;
	};
};

#endif

#ifndef MATCHERAUTOMATON_H
#define MATCHERAUTOMATON_H

#include <vector>
#include <string>

#include "Automaton.h"
#include "Token.h"

class MatcherAutomaton: public Automaton {
private:
	int newLines;
	int match;
	//int charsRead;
	std::string acceptedInput;
	std::vector<std::string> stringMatches {",", ".", "?", "(", ")", "*", "+", ":", ":-", "Schemes", "Facts", "Rules", "Queries"};
	//std::vector<std::string> charMatches { };
	//std::vector<char> charMatches {',', '.', '?', '(', ')', ':', '*', '+'};

	TokenType GetType (int matchIndex) {
		switch (matchIndex) {
			case 0: return COMMA; break;
			case 1: return PERIOD; break;
			case 2: return Q_MARK; break;
			case 3: return LEFT_PAREN; break;
			case 4: return RIGHT_PAREN; break;
			case 5: return MULTIPLY; break;
			case 6: return ADD; break;
			case 7: return COLON; break;
			case 8: return COLON_DASH; break;
			case 9: return SCHEMES; break;
			case 10: return FACTS; break;
			case 11: return RULES; break;
			case 12: return QUERIES; break;
		}
		return ERROR;
	}

public:
	MatcherAutomaton() {};
	~MatcherAutomaton() {};
	int Start(std::string input) {
		match = -1;
		inputRead = 0;
		acceptedInput = "";
		for (unsigned int i = 0; i < stringMatches.size(); i ++) {
			if (i >= 0 && i <= 6) {
				if (input.substr(0, 1).compare(stringMatches.at(i)) == 0) {
					match = i;
					inputRead = 1;
					acceptedInput = input.substr(0, 1);
				}
			} else if (i == 7 || i == 8) {
				if (input.substr(0, 1).compare(stringMatches.at(7)) == 0) {
					match = 7;
					inputRead = 1;
					acceptedInput = input.substr(0, 1);
				} else if (input.substr(0, 2).compare(stringMatches.at(8)) == 0) {
					match = 8;
					inputRead = 2;
					acceptedInput = input.substr(0, 2);
				}
			} else if (i > 8) {
				if (input.substr(0, stringMatches.at(i).size()).compare(stringMatches.at(i)) == 0) {
					match = i;
					inputRead = stringMatches.at(i).size();
					acceptedInput = input.substr(0, stringMatches.at(i).size());
				}
			} else {
				return inputRead;
			}
		}
		/*
		if (match == -1) {
			//return ERROR;
			std::cout << "Nothing matched" << std::endl;
		}
		*/
		return inputRead;
	};
	int NewLinesRead() {
		newLines = 0;

		return newLines;
	};
	Token* CreateToken() {
		Token* token = new Token();
		token->SetString(acceptedInput);
		type = GetType(match);
		token->SetType(type);
		return token;
	};

};

#endif

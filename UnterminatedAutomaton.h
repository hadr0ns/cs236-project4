#ifndef UNTERMINATEDAUTOMATON_H
#define UNTERMINATEDAUTOMATON_H

#include <string>
#include <iostream>

#include "Automaton.h"
#include "Token.h"

class UnterminatedAutomaton: public Automaton {
private:
	std::string unterminated;
public:
	UnterminatedAutomaton() {}
	~UnterminatedAutomaton() {}
	int Start(std::string input) {
		newLines = 0;
		inputRead = 0;
		unterminated = "";
		unsigned int i;

		if (input.at(0) == '\'') {
			for (i = 0; i < input.size(); i++) {
				inputRead++;
				if (input.at(i) == '\''&& i > 0) {
					if (i+1 < input.size()) {
						if (input.at(i+1) == '\''){
							i++;inputRead++;continue;
						} else {
							return 0;
						}
					}
				} else if (input.at(i) == '\n') {
					if (i != input.size()-1) {
						newLines++;
					} else {
						inputRead--;
						continue;
					}
				} else {
					continue;
				}
			}
			unterminated = input;
			return inputRead;
		} else if (input.at(0) == '#' && input.at(1) == '|') {
			inputRead++;
			for (i = 1; i < input.size(); i++) {
				inputRead++;
				if (input.at(i) == '\n') {
					newLines++;
				} else if (input.at(i) == '#') {
					if (input.at(i-1) == '|') {
						return 0;
					} else {
						continue;
					}
				}
			}
			unterminated = input;
			return inputRead;
		}
		unterminated = input;
		return inputRead;
	};
	int NewLinesRead() {
		return newLines;
	};
	Token* CreateToken() {
		//std::cout << "creating unterminated token" << std::endl;
		Token* token = new Token();
		token->SetString(unterminated);
		token->SetType(UNDEFINED);
		return token;
	};

};

#endif

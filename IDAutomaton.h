#ifndef IDAUTOMATON_H
#define IDAUTOMATON_H

#include <string>

#include "Automaton.h"
#include "Token.h"


class IDAutomaton: public Automaton {
private:
	std::string IDString;
public:
	IDAutomaton() {};
	~IDAutomaton() {};
	int Start (std::string input) {
		IDString = "";
		inputRead = 0;
		for (unsigned int i = 0; i < input.size(); i++){
			if (i == 0) {
				if (isalpha(input.at(0))) {
					inputRead++;
				} else {
					return 0;
				}
			} else {
				if (!isalnum(input.at(i))) {
					int u = i;
					if (inputRead != u) {
						std::cout << "Your logic is broken here" << std::endl;
						std::cout << "u is " << u << "; inputRead is " << inputRead << std::endl;
						exit(0);
					}
					IDString = input.substr(0, inputRead);
					return inputRead;
				} else {
					inputRead++;
				}

			}
		}
		return inputRead;
	};
	int NewLinesRead() {
		newLines = 0;
		return newLines;
	};
	Token* CreateToken() {
		Token* token = new Token();
		token->SetString(IDString);
		token->SetType(ID);
		return token;
	};
};
#endif

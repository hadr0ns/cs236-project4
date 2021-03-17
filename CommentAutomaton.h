#ifndef COMMENTAUTOMATON_H
#define COMMENTAUTOMATON_H

#include <string>

#include "Automaton.h"
#include "Token.h"

class CommentAutomaton: public Automaton  {
private:
	std::string comment;
public:
	CommentAutomaton() {}
	~CommentAutomaton() {}
	int Start(std::string input) {
		comment = "";
		inputRead = 0;
		newLines = 0;

		if (input.at(0) == '#') {
			if (input.size() > 1 && input.at(1) == '|') {
				inputRead++; inputRead++;
				for (unsigned int i = 2; i < input.size(); i++) {
					inputRead++;
					if (input.at(i) == '#') {
						if (input.at(i-1) == '|') {
							comment = input.substr(0, inputRead);
							return inputRead;
						} else {
							continue;
						}
					} else if (i == input.size()-1) {
						return 0; //unterminated
					} else if (input.at(i) == '\n') {
						newLines++;
					}
				}
			} else {
				for (unsigned int i = 0; i < input.size(); i ++) {
					if (input.at(i) == '\n') {
						comment = input.substr(0, inputRead);
						return inputRead;
					}
					inputRead++;
				}
			}
		}

		return inputRead;
	};
	int NewLinesRead() {
		return newLines;
	}
	Token* CreateToken() {
		Token* newToken = new Token();
		newToken->SetType(COMMENT);
		newToken->SetString(comment);
		return newToken;
	}

};

#endif

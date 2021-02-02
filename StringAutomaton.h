#ifndef STRINGAUTOMATON_H
#define STRINGAUTOMATON_H

#include "Automaton.h"
#include "Token.h"

class StringAutomaton: public Automaton  {
private:
	std::string stringstring;
public:
	StringAutomaton() {}
	~StringAutomaton() {}
	int Start(std::string input) {
		stringstring = "";
		inputRead = 0;
		newLines = 0;
		if (input.at(0) == '\'') {
			inputRead++;
			//std::cout <<"input.size is " << input.size() << std::endl;
			for (unsigned int i = 1; i < input.size(); i++) {
				//std::cout << "i: "<< i << std::endl;
				inputRead++;
				if (input.at(i) == '\'') {
					if (i == 1) {
						if (input.at(i+1) != '\'') {
							stringstring = input.substr(0, inputRead);
							return inputRead;
						} else {
							i++; inputRead++; continue;
						}
					} else if (i > 1) {
						if (i+1 < input.size()) { //check to avoid seg fault
							if (input.at(i+1) == '\''){
								i++; inputRead++; continue;
							} else {
								stringstring = input.substr(0, inputRead);
								return inputRead;
							}
						}
					} /*else {
						stringstring = input.substr(0, inputRead);
						return inputRead;
					}*/
				} else if (input.at(i) == '\n') {
					newLines++;
					//inputRead++;
				}
				if (i == input.size()-1) {
					//std::cout << "made it!" << std::endl;
					return 0;
				}
			}
		} else {
			return 0;
		}
		return inputRead;
	};
	int NewLinesRead() {
		return newLines;
	}
	Token* CreateToken() {
		Token* newToken = new Token();
		newToken->SetType(STRING);
		newToken->SetString(stringstring);
		return newToken;
	}

};

#endif

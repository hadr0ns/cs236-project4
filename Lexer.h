#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>

#include "Automaton.h"
#include "Token.h"

#include "MatcherAutomaton.h"

class Lexer {
private:
	std::vector<Token*> tokens;
	std::vector<Automaton*> automata;
	int lineNumber;
public:
	Lexer() {
		//tokens = new std::vector<Token*>;
		//automata = new std::vector<Automaton*>;
		//add all automaton instances
		automata.push_back(new MatcherAutomaton());
	};
	~Lexer(){};
	void Run(std::string input) {
		lineNumber = 1;
		while (input.size() > 0) {
			int maxRead = 0;
			Automaton* maxAutomaton = automata.at(0);

			//handle whitespace in between tokens
			if (isspace(input.at(0))) {
				if (input.at(0) == "\n") {
					lineNumber++;
					input.erase(input.at(0));
				} else if (input.at(0) == "\t") {
					input.erase(input.at(0));
				} else if (input.at(0) == " ") {
					//figure this out for spaces;
				} else {
					std::cout << "Error in the whitespace function" << std::endl;
					exit(0);
				}
			}

			for (unsigned int i = 0; i < automata.size(); i++) {
				int inputRead = automata.at(i)->Start(input);
				if (inputRead > maxRead) {
					maxRead = inputRead;
					maxAutomaton = automata.at(i);
				}
			}
			if (maxRead > 0) {
				Token* newToken = maxAutomaton->CreateToken();
				tokens.push_back(newToken);
				lineNumber += maxAutomaton->NewLinesRead();
			}
		}
		TokenType* newType = new Token();
		newType.SetType(ENDOF);
		newType.SetString("");
		newType.SetLineNumber(lineNumber);
		//this may not be right; possibly linenumber + 1. 
	};
	//other methods


};

#endif

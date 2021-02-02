#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include <cstring>

#include "Automaton.h"
#include "Token.h"

#include "MatcherAutomaton.h"
#include "UndefinedAutomaton.h"

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
		automata.push_back(new UndefinedAutomaton());
	};
	~Lexer(){};
	void Run(std::string input) {
		lineNumber = 1;
		while (input.size() > 0) {
			int maxRead = 0;
			Automaton* maxAutomaton = automata.at(0);

			//handle whitespace in between tokens
			if (isspace(input[0])) {
				if (input.substr(0, 1).compare("\n") == 0) {
					lineNumber++;
					input.erase(input.begin()+0);
				} else if (input.substr(0, 1).compare("\t") == 0) {
					input.erase(input.begin()+0);
				} else if (input.substr(0, 1).compare(" ") == 0) {
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
				newToken->SetLineNumber(lineNumber);
				tokens.push_back(newToken);
				lineNumber += maxAutomaton->NewLinesRead();
				input.erase(0, maxRead);
			}
		}
		Token* newType = new Token();
		newType->SetType(ENDFILE);
		newType->SetString("");
		newType->SetLineNumber(lineNumber);
		tokens.push_back(newType);
		//this may not be right; possibly linenumber + 1.

		for (unsigned int i = 0; i < tokens.size(); i++) {
			std::cout << tokens.at(i)->to_string() << std::endl;
		}
	};
	//other methods


};

#endif

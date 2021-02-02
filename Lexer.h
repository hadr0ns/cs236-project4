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
#include "IDAutomaton.h"
#include "StringAutomaton.h"
#include "UnterminatedAutomaton.h"
#include "CommentAutomaton.h"

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
		automata.push_back(new MatcherAutomaton()); //0
		automata.push_back(new IDAutomaton()); //1; this will be skipped if matcher comes back positive
		automata.push_back(new StringAutomaton()); //2
		automata.push_back(new CommentAutomaton()); //3
		automata.push_back(new UnterminatedAutomaton()); //4
		automata.push_back(new UndefinedAutomaton()); //5
	};
	~Lexer(){};
	void Run(std::string input) {
		lineNumber = 1;
		while (input.size() > 0) {
			int maxRead = 0;
			Automaton* maxAutomaton = automata.at(0);

			//handle whitespace in between tokens
			if (isspace(input.at(0))) {
				//std::cout << "made it into the whitespace fn" << std::endl;
				if (input.at(0) == '\n') {
					lineNumber++;
					input.erase(0, 1);
				} else if (input.at(0) == '\t') {
					input.erase(0, 1);
				} else if (input.at(0) == ' ') {
					//figure this out for spaces;
					//std::cout << "made it to a space" << std::endl;
					input.erase(0, 1);
				} else {
					std::cout << "Error in the whitespace function" << std::endl;
					exit(0);
				}
				continue;
			}

			for (unsigned int i = 0; i < automata.size(); i++) {
				if (i == 1) {
					if (automata.at(0)->Start(input)>2) {
						i = 2;
					}
				}
				int inputRead = automata.at(i)->Start(input);
				//std::cout << "InputRead at " << i << ": " << inputRead << std::endl;
				if (inputRead > maxRead) {
					maxAutomaton = automata.at(i);
					maxRead = inputRead;
				}
			}
			if (maxRead > 0) {
				Token* newToken = maxAutomaton->CreateToken();
				newToken->SetLineNumber(lineNumber);
				//std::cout << newToken->to_string() << std::endl;
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
		std::cout << "Total Tokens = " << tokens.size() << std::endl;
	};
	//other methods


};

#endif

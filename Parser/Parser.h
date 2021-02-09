#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>

#include "../Lexer/Token.h"
#include "Facts.h"
#include "Queries.h"
#include "Rules.h"
#include "Schemes.h"

class Parser {
private:
	std::vector<Token*> tokens;
	std::vector<Schemes*> schemes;
	std::vector<Facts*> facts;
	std::vector<Rules*> rules;
	std::vector<Queries*> queries;

public:
	Parser() {};
	~Parser() {};
	void Run() {
		bool accepted = true;
		//int undefined = -1;
		for (unsigned int i = 0; i < tokens.size(); i++) {
			if (tokens.at(i)->GetType() == UNDEFINED) {
				accepted = false;
				//undefined = i;
				break;
			} else {
				continue;
			}
		}
		if (accepted) {
			std::cout << "Success!" << std::endl;
		} else {
			std::cout << "Failure!" << std::endl;
		}
	};
	void InputTokens(std::vector<Token*> inputTokens) {
		//also strips comments
		for(unsigned int i = 0; i < inputTokens.size(); i++){
			if (inputTokens.at(i)->GetType() == COMMENT) {
				continue;
			} else {
				tokens.at(i) = inputTokens.at(i);
			}
		}
		return;
	};
};

#endif

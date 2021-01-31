#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <string>
#include <iostream>
#include "Token.h"

class Automaton {
protected:
	int inputRead = 0;
	int newLines = 0;
	TokenType type;
public:
	//Automaton() {}
	virtual ~Automaton() {}
	virtual int Start(std::string input)=0;
	virtual int NewLinesRead()=0;
	virtual Token* CreateToken()=0;

};

#endif

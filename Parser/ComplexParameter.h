#ifndef COMPLEXPARAMETER_H
#define COMPLEXPARAMETER_H

#include <iostream>
#include <string>
#include <vector>
#include "Parameter.h"

class ComplexParameter: public Parameter {
private:
	Parameter* parameter1;
	std::string operatorString;
	Parameter* parameter2;
public:
	ComplexParameter() {};
	~ComplexParameter() {};
	std::string to_string() {
		return "(" + parameter1->to_string() + operatorString + parameter2->to_string() + ")";
	};
	void SetParameter1(Parameter* input) {
		parameter1 = input;
	};
	void SetParameter2(Parameter* input) {
		parameter2 = input;
	};
	void SetOperator(std::string input) {
		operatorString = input;
	};

};

#endif

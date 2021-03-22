#ifndef SIMPLEPARAMETER_H
#define SIMPLEPARAMETER_H

#include <iostream>
#include <string>
#include "Parameter.h"

class SimpleParameter: public Parameter {
private:
	std::string parameter;
public:
	SimpleParameter() {};
	~SimpleParameter() {};
	void SetParameter(std::string input) {
		parameter = input;
	};
	std::string to_string() {
		return parameter;
	};
	std::string GetName() {
		return parameter;
	}

};

#endif

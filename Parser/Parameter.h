#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>
#include <string>

enum ParameterType {IDD, STR, EXPRESSION};

class Parameter {
private:
	ParameterType parameterType;
public:
	//Parameter() {};
	virtual ~Parameter() {};
	virtual std::string to_string() {
		return "default";
	}
	void SetType(ParameterType input) {
		parameterType = input;
	}

};

#endif

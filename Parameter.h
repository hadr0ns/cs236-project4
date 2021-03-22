#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>
#include <string>

enum ParameterType {IDD, STR, EXPRESSION};

class Parameter {
private:
	ParameterType parameterType;
	bool isConstant;
public:
	//Parameter() {};
	virtual ~Parameter() {};
	virtual std::string to_string() {
		return "default";
	}
	void SetType(ParameterType input) {
		parameterType = input;
		if (parameterType == STR) {
			isConstant = true;
		} else if (parameterType == IDD) {
			isConstant = false;
		} else {
			isConstant = false;
		}
	}
	bool IsConstant() {
		return isConstant;
	}
	virtual std::string GetName() {
		return "default";
	}

};

#endif

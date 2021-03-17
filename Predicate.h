#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>
#include <string>
#include <vector>

#include "Parameter.h"

class Predicate {
private:
	std::string name;
	std::vector<Parameter*> body;
public:
	Predicate() {};
	~Predicate() {};
	void AddName(std::string input) {
		name = input;
	};
	void AddToBody(Parameter* input) {
		body.push_back(input);
	};
	std::string to_string() {
		std::string stringVersion = name + "(";
		for (unsigned int i = 0; i < body.size(); i++) {
			stringVersion+=body.at(i)->to_string();
			if (i != body.size()-1) {
				stringVersion+= ",";
			}
		}
		stringVersion+=")";
		return stringVersion;
	};
	std::vector<Parameter*> GetBody() {
		return body;
	}
	std::string GetName() {
		return name;
	}

};

#endif

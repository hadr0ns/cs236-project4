#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>
#include <string>

//#include "Parameter.h"

class Predicate {
private:
	std::string headPredicate;
public:
	Predicate() {};
	~Predicate() {};
	std::string to_string() {
		return "string";
	};
};

#endif

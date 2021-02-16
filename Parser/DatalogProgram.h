#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <iostream>
#include <vector>
#include <string>

#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"

class DatalogProgram {
private:
	std::vector<Predicate*> schemes;
	std::vector<Predicate*> facts;
	std::vector<Predicate*> queries;
	std::vector<Rule*> rules;

public:
	DatalogProgram() {};
	~DatalogProgram() {};
	std::string to_string() {
		return "yeetus defeatus";
	};
};

#endif

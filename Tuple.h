#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>
#include <sstream>

#include "Relation.h"

class Tuple {
protected:
	std::vector<std::string> values;
	std::string associatedName;
public:
	Tuple() {};
	//TODO: this is problematic, leading to duplicated entries.
	bool operator< (const Tuple & other) const{
		//compare this tuple to another tuple i guess?
		return this->values < other.values;
	};
	void SetName(std::string name) {
		associatedName = name;
	}
	std::string GetName() {
		return associatedName;
	}
	void AddValue(std::string value) {
		values.push_back(value);
	}
	std::string to_string() {
		std::stringstream ss;
		for (unsigned int i = 0; i < values.size(); i++) {
			ss<<values.at(i)<<"\n";
		}
		return ss.str();
	}
	std::string GetColumn(int index) {
		return values.at(index);
	}
	std::vector<std::string> GetValues() {
		return values;
	}
};

#endif

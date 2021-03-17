#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <vector>
#include <set>
#include <list>
#include <sstream>

#include "Tuple.h"
#include "Header.h"

class Relation {
private:
	std::set<Tuple*> tuples;
	std::string name;
	Header* header;

public:
	Relation(){};
	void Select(int index, std::string value) {

	};
	void Select(int index1, int index2) {

	};
	void Project(std::list<int> indices) {

	};
	void Rename(std::list<std::string> attributes){

	};
	void SetName(std::string string) {
		name = string;
	}
	void SetHeader(Header* input) {
		header = input;
	}
	void AddTuple(Tuple* tuple) {
		tuples.insert(tuple);
	}
	std::string GetName() {
		return name;
	}
 	std::string to_string() {
		std::stringstream ss;
		ss<<header->to_string();
		ss<<"Tuples: ";
		int i = 0;
		for (auto elem : tuples) {
			ss<<i<<": "<< elem->to_string();
			i++;
		}
		return ss.str();
	}

};

#endif

//std::set<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); it++)

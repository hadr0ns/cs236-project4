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
	//should these be void? or a vector of tuples lol--ans:modifying an existing relation to match
	Relation* Select(int index, std::string value) {
		//std::cout << this->to_string()<<std::endl;
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			std::cout<<elem->to_string()<<std::endl;

			if (elem->GetColumn(index) == value) {
				returnRelation->AddTuple(elem);
			}
		}
		//PrintRemainingTuples();
		return returnRelation;
	};
	Relation* Select(int index1, int index2) {
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			if (elem->GetColumn(index1) == elem->GetColumn(index2)) {
				returnRelation->AddTuple(elem);
			}
		}
		//PrintRemainingTuples();
		return returnRelation;
	};
	Relation* Project(std::vector<int> indices) {
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			Tuple* newTuple = new Tuple();
			for (unsigned int i = 0; i < indices.size(); i++) {
				newTuple->AddValue(elem->GetColumn(i));
			}
			returnRelation->AddTuple(newTuple);
		}
		return returnRelation;
	};
	Relation* Rename(std::vector<std::string> attributes){
		Relation* returnRelation = new Relation();
		Header* header = new Header();
		for (auto elem : attributes) {
			header->AddAttribute(elem);
		}
		return returnRelation;

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
	void PrintRemainingTuples() {
		std::cout << "Remaining:" <<std::endl;
		for (auto elem : tuples) {
			std::cout << elem->to_string()<<std::endl;
		}
	}
	std::string evaluated_to_string() {
		std::stringstream ss;

		return ss.str();
	}

};

#endif

//std::set<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); it++)

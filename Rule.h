#ifndef RULE_H
#define RULE_H

#include <iostream>
#include <vector>
#include <string>
#include "Predicate.h"

class Rule {
private:
	Predicate* headPredicate;
	std::vector<Predicate*> predicateList;
public:
	Rule() {};
	~Rule() {};
	void AddPredicate(Predicate* predicate) {
		predicateList.push_back(predicate);
	};
	void SetHeadPredicate(Predicate* predicate) {
		headPredicate = predicate;
	}
	std::string to_string() {
		std::string stringVersion = headPredicate->to_string();
		stringVersion+= " :- ";
		for (unsigned int i = 0; i < predicateList.size(); i++) {
			stringVersion+=predicateList.at(i)->to_string();
			if(i != predicateList.size()-1) {
				stringVersion+=",";
			}
		}
		stringVersion+=".";
		return stringVersion;
	}
	std::vector<Predicate*> GetPredicateList() {
		return predicateList;
	}
	Predicate* GetHeadPredicate() {
		return headPredicate;
	}
};

#endif

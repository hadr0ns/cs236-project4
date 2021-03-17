#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>

#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"

class DatalogProgram {
private:
	std::vector<Predicate*> schemes;
	std::vector<Predicate*> facts;
	std::vector<Rule*> rules;
	std::vector<Predicate*> queries;

	std::set<std::string> domain;

public:
	DatalogProgram() {};
	~DatalogProgram() {};
	std::string to_string() {
		std::stringstream ss;
		ss << "Schemes(";
		ss << schemes.size();
		ss << "):\n";
		for (unsigned int i = 0; i < schemes.size(); i++) {
			ss << "\t";
			ss << schemes.at(i)->to_string();
			ss << "\n";
		}
		ss << "Facts(";
		ss << facts.size();
		ss << "):\n";
		for (unsigned int i = 0; i < facts.size(); i++) {
			ss << "\t";
			ss << facts.at(i)->to_string();
			ss << ".\n";
		}
		ss << "Rules(";
		ss << rules.size();
		ss << "):\n";
		for (unsigned int i = 0; i < rules.size(); i++) {
			ss << "\t";
			ss << rules.at(i)->to_string();
			ss << "\n";
		}
		ss << "Queries(";
		ss << queries.size();
		ss << "):\n";
		for (unsigned int i = 0; i < queries.size(); i++) {
			ss << "\t";
			ss << queries.at(i)->to_string();
			ss << "?\n";
		}
		ss << "Domain(";
		ss << domain.size();
		ss << "):\n";
		for (std::set<std::string>::iterator i = domain.begin(); i != domain.end(); i++) {
			ss << "\t";
			ss << *i;
			ss << "\n";
		}
		return ss.str();
	};
	void AddScheme(Predicate* scheme) {
		schemes.push_back(scheme);
	};
	void AddFact(Predicate* fact) {
		facts.push_back(fact);
		for (unsigned int i = 0; i < fact->GetBody().size(); i++) {
			domain.insert(fact->GetBody().at(i)->to_string());
		}
	};
	void AddRule(Rule* rule) {
		rules.push_back(rule);
	};
	void AddQuery(Predicate* query) {
		queries.push_back(query);
	}
	std::vector<Predicate*> GetSchemes() {
		return schemes;
	}
	std::vector<Predicate*> GetFacts() {
		return facts;
	}
	std::vector<Predicate*> GetQueries() {
		return queries;
	}

};

#endif

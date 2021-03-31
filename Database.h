#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "Predicate.h"
#include "DatalogProgram.h"
#include "Relation.h"
#include "Header.h"
#include "Parameter.h"
#include "Rule.h"

class Database {
private:
	std::map<std::string, Relation*> databaseMap;
	std::vector<Predicate*> schemes;
	std::vector<Predicate*> facts;
	std::vector<Predicate*> queries;
	std::vector<Header*> headers;
	std::vector<Tuple> tuples;
	std::vector<Rule*> rules;
public:
	Database() {};
	void Build(DatalogProgram* program) {
		schemes = program->GetSchemes();
		facts = program->GetFacts();
		queries = program->GetQueries();
		rules = program->GetRules();
		BuildSchemes();

		//std::cout<<to_string()<<std::endl;
	}
	void BuildSchemes() {
		for (unsigned int i = 0; i < schemes.size(); i++) {
			Header* header = new Header();
			header->SetName(schemes.at(i)->GetName());
			std::vector<Parameter*> parameters = schemes.at(i)->GetBody();
			for (unsigned int j = 0; j < parameters.size(); j++) {
				header->AddAttribute(parameters.at(j)->to_string());
			}
			headers.push_back(header);
		}

		for (unsigned int i = 0; i < facts.size(); i++) {
			Tuple tuple;
			tuple.SetName(facts.at(i)->GetName());
			std::vector<Parameter*> parameters;
			parameters = facts.at(i)->GetBody();
			for (unsigned int j = 0; j < parameters.size(); j++) {
				tuple.AddValue(parameters.at(j)->to_string());
			}
			tuples.push_back(tuple);
		}

		for (unsigned int i = 0; i < headers.size(); i++) {
			Relation* relation = new Relation();
			std::string relationName = headers.at(i)->GetName();
			relation->SetName(relationName);
			relation->SetHeader(headers.at(i));
			for (unsigned int j = 0; j < tuples.size(); j++) {
				if (tuples.at(j).GetName() == relationName) {
					relation->AddTuple(tuples.at(j));
				}
			}
			databaseMap.insert({relationName, relation});
		}
	}
	std::string to_string() {
		std::stringstream ss;
		for (std::map<std::string, Relation*>::const_iterator it = databaseMap.begin(); it != databaseMap.end(); it++) {
			ss << it->first << ": " << it->second->to_string() << std::endl;
		}
		return ss.str();
	}

	std::vector<Predicate*> GetQueries() {
		return queries;
	}
	Relation* GetMatchingRelation(std::string name) {
		for (std::map<std::string, Relation*>::iterator it = databaseMap.begin(); it != databaseMap.end(); it++) {
			if (it->first == name) {
				return it->second;
			}
		}
		return 0;
	}
	std::vector<Rule*> GetRules() {
		return rules;
	}
	bool UnionToDatabase(std::vector<Relation*> evaluatedRules) {
		bool added = false;
		for (auto pair : databaseMap) {
			std::set<Tuple> tuples = pair.second->GetTuples();
			std::set<Tuple> evaluatedRulesTuples = evaluatedRules->GetTuples();
			for (auto elem : evaluatedRulesTuples) {
				if (tuples.insert(elem).second) {
					added = true;
				}
			}
		}
		return added;
	}

};

#endif

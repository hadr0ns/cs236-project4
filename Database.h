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
	std::set<Tuple> newTuples;
	bool addedglobal;
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
			relation->SetName(relationName);
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
		newTuples.clear();
		bool added = false;
		for (unsigned int i = 0; i < evaluatedRules.size(); i++) {
			std::set<Tuple> evaluatedRulesTuples = evaluatedRules.at(i)->GetTuples();
			std::string name = evaluatedRules.at(i)->GetName();
			Relation* matchingRelation = GetMatchingRelation(name);
			std::set<Tuple> tuples = matchingRelation->GetTuples();
			for (auto elem : evaluatedRulesTuples) {
				if (tuples.insert(elem).second) {
					added = true;
					matchingRelation->AddTuple(elem);
					newTuples.insert(elem);
				}
			}
		}
		return added;
	}
	bool UnionToDatabaseSingle(Relation* evaluatedRule) {
		newTuples.clear();
		bool added = false;
		std::set<Tuple> evaluatedRuleTuples = evaluatedRule->GetTuples();
		std::string name = evaluatedRule->GetName();
		Relation* matchingRelation = GetMatchingRelation(name);
		//std::cout << "matching relation is " << matchingRelation->to_string();
		std::set<Tuple> tuples = matchingRelation->GetTuples();
		for (auto elem : evaluatedRuleTuples) {
			if (tuples.insert(elem).second) {
				added = true;
				if (added) {
					addedglobal = true;
				}
				matchingRelation->AddTuple(elem);
				newTuples.insert(elem);
			}
		}
		//std::cout << to_string();
		return added;
	}
	bool AddedGlobal() {
		return addedglobal;
	}
	std::set<Tuple> GetNewTuples() {
		return newTuples;
	}
	void AddedGlobalFalse() {
		addedglobal = false;
	}

};

#endif

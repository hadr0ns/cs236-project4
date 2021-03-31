#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include "DatalogProgram.h"
#include "Token.h"
#include "Database.h"
#include "Predicate.h"
#include "Relation.h"
#include "Parameter.h"
#include "Header.h"

class Interpreter {
private:
	Database* database;
	std::vector<Relation*> evaluatedQueries;
	std::vector<Predicate*> queries;
	std::vector<Rule*> rules;
	std::vector<Relation*> evaluatedRules;

public:
	Interpreter(){};
	void Run(DatalogProgram* program) {
		database = new Database();
		database->Build(program);
		//std::cout << database->to_string()<<std::endl;
		rules = database->GetRules();
		EvaluateRules();
		//std::cout << database->to_string();
		queries = database->GetQueries();
		EvaluateQueries();

	};
	void EvaluateRules() {
		//need to set this up to loop maybe? --doing that earlier
		int counter = 1;
		bool added = false;
		std::cout << "Rule Evaluation" << std::endl;
		do {
			database->AddedGlobalFalse();
			std::vector<Relation*> rhs;
			for (unsigned int i = 0; i < rules.size(); i++) {
				Relation* newEvaluatedRule = new Relation();
				newEvaluatedRule = EvaluateRule(rules.at(i));
				evaluatedRules.push_back(newEvaluatedRule);
				added = database->UnionToDatabaseSingle(newEvaluatedRule);
				//printing
				std::set<Tuple> addedTuples = database->GetNewTuples();
				std::cout << rules.at(i)->to_string();
				Relation* printRelation = new Relation();
				for (auto elem : addedTuples) {
					printRelation->AddTuple(elem);
				}
				Header* header = newEvaluatedRule->GetHeader();
				printRelation->SetHeader(header);
				std::string name = newEvaluatedRule->GetName();
				printRelation->SetName(name);
				std::cout << printRelation->evaluated_rule_to_string()<<std::endl;
			}
			//added = database->UnionToDatabase(evaluatedRules);
			added = database->AddedGlobal();
			if (added) {
				counter++;
			}
			evaluatedRules.clear();
			//if i run into errors here i may need to clear the evaluatedRules vector between passes. should it matter?
		} while (added);
		std::cout << "\nSchemes populated after " << counter << " passes through the Rules.\n" << std::endl;

	};
	Relation* EvaluateRule(Rule* r) {
		std::vector<Predicate*> predicateList = r->GetPredicateList();
		Predicate* headPredicate = r->GetHeadPredicate();
		std::vector<Relation*> rhs;
		//get the predicates out and evaluated
		for (unsigned int i = 0; i < predicateList.size(); i++) {
			//*DEBUG
			//std::cout << predicateList.at(i)->to_string() << std::endl;
			//END DEBUG */
			Relation* newRelation = evaluatePredicate(predicateList.at(i));
			//std::cout << "80i: " << i << newRelation->to_string();
			rhs.push_back(newRelation);
		}
		//join rhs loop
		Relation* intRelation = new Relation();
		if (rhs.size() > 1) {
			for (unsigned int j = 0; j < rhs.size(); j++) {
				if (j == 0) {
					intRelation = rhs.at(0)->Join(rhs.at(1));
					j++;
				} else if (j == 1) {
					std::cout << "you messed up";
					exit(0);
				} else {
					intRelation = intRelation->Join(rhs.at(j));
				}
				//std::cout << "intRelation at j = " << j << ": " << intRelation->to_string();
			}
		} else {
			intRelation = rhs.at(0);
		}

		//project on the new relation to get the columns in the right order
		std::vector<int> indices;
		std::vector<Parameter*> headPredicateBody = headPredicate->GetBody();
		std::string headPredicateName = headPredicate->GetName();
		Header* intHeader = intRelation->GetHeader();
		std::vector<std::string> intAttributes = intHeader->GetAttributes();
		Relation* returnRelation = new Relation();
		std::vector<std::string> headPredicateBodyNames;
		for (unsigned int i = 0; i < headPredicateBody.size(); i++) {
			headPredicateBodyNames.push_back(headPredicateBody.at(i)->GetName());
		}
		for (unsigned int i = 0; i < headPredicateBodyNames.size(); i++) {
			for (unsigned int j = 0; j < intAttributes.size(); j++) {
				if (headPredicateBodyNames.at(i)==intAttributes.at(j)) {
					indices.push_back(j);
				}
			}
		}
		returnRelation = intRelation->Project(indices);
		Relation* matchingRelation = database->GetMatchingRelation(headPredicateName);
		Header* matchingRelationHeader = matchingRelation->GetHeader();
		std::vector<std::string> matchingRelationHeaderAttributes = matchingRelationHeader->GetAttributes();
		returnRelation = returnRelation->Rename(matchingRelationHeaderAttributes);
		returnRelation->SetName(headPredicateName);
		//std::cout << "line 113" << returnRelation->to_string()<<std::endl;

		return returnRelation;
	}
	void EvaluateQueries() {
		std::cout << "Query Evaluation" << std::endl;
		for (unsigned int i = 0; i < queries.size(); i++) {
			//Relation* newRelation = new Relation();
			Relation* newRelation = evaluatePredicate(queries.at(i));
			evaluatedQueries.push_back(newRelation);
			Print(queries.at(i), newRelation);
		}
	};
	Relation* evaluatePredicate(Predicate* p) {
		//std::cout << "evaluating predicate" << std::endl;
		//std::cout << database->to_string()<<std::endl;
		Relation* matchingRelation = new Relation();
		//Relation* returnRelation = new Relation();
		//if there's a problem this is the problem remember
		matchingRelation = database->GetMatchingRelation(p->GetName());
		std::map<std::string, int> seenVariables;
		std::vector<std::string> seenVariablesStrings;
		std::vector<int> seenVariablesIndices;
		std::vector<Parameter*> queryBody = p->GetBody();
		for (unsigned int i = 0; i < queryBody.size(); i++) {
			std::string nameAtI = queryBody.at(i)->GetName();
			if (p->GetBody().at(i)->IsConstant()) {
				matchingRelation = matchingRelation->Select(i, nameAtI);
			} else {
				bool seen = false;
				for (auto elem : seenVariables) {
					if (elem.first == nameAtI) {
						//have we seen this variable before?
						seen = true;
						matchingRelation = matchingRelation->Select(elem.second, i);
					}
				}
				if (!seen) {
					seenVariables.insert({nameAtI, i});
					seenVariablesStrings.push_back(nameAtI);
					seenVariablesIndices.push_back(i);
				}
			}
		}
		matchingRelation = matchingRelation->Project(seenVariablesIndices);
		matchingRelation = matchingRelation->Rename(seenVariablesStrings);
		return matchingRelation;
	};
	void Print(Predicate* query, Relation* relation) {
		std::cout << query->to_string()<< "? ";
		std::cout << relation->evaluated_to_string() <<std::endl;
	};
};

#endif

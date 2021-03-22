#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include "DatalogProgram.h"
#include "Token.h"
#include "Database.h"
#include "Predicate.h"
#include "Relation.h"
#include "Parameter.h"

class Interpreter {
private:
	Database* database;
	std::vector<Relation*> evaluatedQueries;
	std::vector<Predicate*> queries;
public:
	Interpreter(){};
	void Run(DatalogProgram* program) {
		database = new Database();
		database->Build(program);
		//std::cout << database->to_string()<<std::endl;
		queries = database->GetQueries();
		EvaluateQueries();

	};
	void EvaluateQueries() {
		for (unsigned int i = 0; i < queries.size(); i++) {
			//Relation* newRelation = new Relation();
			Relation* newRelation = evaluatePredicate(queries.at(i));
			evaluatedQueries.push_back(newRelation);
			Print(queries.at(i), newRelation);
		}
	}
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
	}
};

#endif

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include "DatalogProgram.h"
#include "Token.h"
#include "Database.h"
#include "Predicate.h"
#include "Relation.h"

class Interpreter {
private:
	Database* database;
	std::vector<Relation*> evaluatedQueries;
public:
	Interpreter(){};
	void Run(DatalogProgram* program) {
		Database* database = new Database();
		database->Build(program);
		std::vector<Predicate*> queries = database->GetQueries();
		for (unsigned int i = 0; i < queries.size(); i++) {
			Relation* newRelation = new Relation();
			newRelation = evaluatePredicate(queries.at(i));
			evaluatedQueries.push_back(newRelation);
		}

	};
	Relation* evaluatePredicate(Predicate* p) {
		Relation* relation = new Relation();

		return relation;
	};
};

#endif

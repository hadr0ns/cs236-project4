#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <sstream>

#include "Token.h"
#include "Rule.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Parameter.h"
#include "SimpleParameter.h"
#include "ComplexParameter.h"

class Parser {
private:
	std::vector<Token*> tokens;
	DatalogProgram* datalog;
	bool accepted;
	bool hasSchemes;
	bool hasFacts;
	bool hasRules;
	bool hasQueries;
	int rejected;
	/*
	Predicate* query;
	Predicate* passAlongPredicate;
	Parameter* passAlongParameter;
	*/
	//Rule* newRule;
public:
	Parser() {};
	~Parser() {};

	void Run(){
		datalog = new DatalogProgram();
		accepted = true;
		rejected = -1;
		hasSchemes = false;
		hasFacts = false;
		hasRules = false;
		hasQueries = false;
		//for loop is bad idea here I think
		//
		//[ this is the part that checks for undefined tokens and breaks there

		for (unsigned int i = 0; i < tokens.size(); i++) {
			if (tokens.at(i)->GetType() == UNDEFINED) {
				accepted = false;
				rejected = i;
				break;
			} else {
				continue;
			}
		}

		int i = 0;
		//start checking for following grammar
		//check to make sure schemes is first
		if (hasSchemes == false) {
			if (tokens.at(i)->GetType() == SCHEMES) {
				hasSchemes = true;
				i++;
				if (tokens.at(i)->GetType() == COLON) {
					i++;
				} else {
					rejected = i;
					accepted = false;
					ReturnState();
				}
				i = Schemes(i);
				if (!accepted) {
					ReturnState();
					return;
				}
			} else {
				accepted = false;
				rejected = i;
				ReturnState();
				return;
			}
		}
		if (tokens.at(i)->GetType() == FACTS) {
			hasFacts = true;
			i++;
			if (tokens.at(i)->GetType() == COLON) {
				i++;
			} else {
				rejected = i;
				accepted = false;
				ReturnState();
			}
			if (tokens.at(i)->GetType() != RULES) {
				i = Facts(i);
				if (!accepted) {
					ReturnState();
					return;
				}
			}
		}

		if (tokens.at(i)->GetType() == RULES) {
			hasRules = true;
			if (!hasFacts) {
				rejected = i;
				accepted = false;
				ReturnState();
				return;
			}
			i++;
			if (tokens.at(i)->GetType() == COLON) {
				i++;
			} else {
				rejected = i;
				accepted = false;
				ReturnState();
			}
			if (tokens.at(i)->GetType() != QUERIES) {
				i = Rules(i);
				if (!accepted) {
					ReturnState();
					return;
				}
			}
		}
		if (tokens.at(i)->GetType() == QUERIES) {
			hasQueries = true;
			if(!hasRules) {
				rejected = i;
				accepted = false;
				ReturnState();
				return;
			}
			i++;
			if (tokens.at(i)->GetType() == COLON) {
				i++;
			} else {
				rejected = i;
				accepted = false;
				ReturnState();
				return;
			}
			i = Queries(i);
			if (!accepted) {
				ReturnState();
				return;
			}
		} else {
			rejected = i;
			accepted = false;
			ReturnState();
			return;
		}
		if (tokens.at(i)->GetType() == ENDFILE) {
			//end--this is the good ending
			ReturnState();
			return;
		} else {
			accepted = false;
			rejected = i;
			ReturnState();
			return;
		}
	};

	int Schemes(int index){
		//schemes()--runs Scheme() until end of SchemeList, until it hits Facts().
		int i = index;
		i = Scheme(i);
		if (!accepted) {
			return i;
		}
		bool schemeList = false;
		do {
			if (tokens.at(i)->GetType() == ID) {
				i++;
			} else {
				return i;
			}
			if (tokens.at(i)->GetType() == LEFT_PAREN && tokens.at(i+1)->GetType() == ID) {
				schemeList = true;
				i = Scheme(i-1);
				if (!accepted) {
					schemeList = false;
					return i;
				}
			} else {
				return i-1;
			}
		} while (schemeList);

		std::cout << "ended at Schemes()" << std::endl;
		exit(0);
		return 0;
	};
	int Scheme(int index){
		Predicate* newScheme = new Predicate();
		//ID LEFT_PAREN ID idList RIGHT_PAREN
		//idList-> COMMA ID idList
		int i = index;
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
			newScheme->AddName(tokens.at(i)->GetString());
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == ID) {
			//idList
			SimpleParameter* parameter1 = new SimpleParameter();
			parameter1->SetType(IDD);
			parameter1->SetParameter(tokens.at(i)->GetString());
			newScheme->AddToBody(parameter1);
			i++;
			while (accepted) {
				if (tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == ID) {
						SimpleParameter* parameter = new SimpleParameter();
						parameter->SetType(IDD);
						parameter->SetParameter(tokens.at(i)->GetString());
						newScheme->AddToBody(parameter);
						i++;
						continue;
					} else {
						rejected = i;
						accepted = false;
						return rejected;
					}
				} else if (tokens.at(i)->GetType() == RIGHT_PAREN) {
					i++;
					datalog->AddScheme(newScheme);
					return i;
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
			}

		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		std::cout << "how in the hell schemes" << std:: endl;
		exit(0);
		return 0;
	};

	int Facts(int index){
		int i = index;
		i = Fact(i);
		if (!accepted) {
			return i;
		}
		bool factList = false;
		do {
			if (tokens.at(i)->GetType() == ID) {
				factList = true;
				i = Fact(i);
				if (!accepted) {
					return i;
				}
			} else {
				return i;
			}

		} while (factList);

		std::cout << "ended at Facts()" << std::endl;
		exit(0);
		return 0;
	};
	int Fact(int index){
		//ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
		//stringList -> COMMA STRING stringList
		int i = index;
		Predicate* newFact = new Predicate();
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
			newFact->AddName(tokens.at(i)->GetString());
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == STRING) {
			//StringList
			SimpleParameter* parameter1 = new SimpleParameter();
			parameter1->SetType(STR);
			parameter1->SetParameter(tokens.at(i)->GetString());
			newFact->AddToBody(parameter1);
			i++;
			while (accepted) {
				if (tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == STRING) {
						SimpleParameter* parameter = new SimpleParameter();
						parameter->SetType(STR);
						parameter->SetParameter(tokens.at(i)->GetString());
						newFact->AddToBody(parameter);
						i++;
						continue;
					} else {
						rejected = i;
						accepted = false;
						return rejected;
					}
				} else if (tokens.at(i)->GetType() == RIGHT_PAREN) {
					i++;
					break;
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
			}

		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == PERIOD) {
			datalog->AddFact(newFact);
			i++;
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		std::cout << "how in the hell facts" << std:: endl;
		exit(0);
		return 0;
	};

	int Rules(int index){
		int i = index;
		if (tokens.at(i)->GetType() == ID) {
			i = RuleHandler(i);
			if (!accepted) {
				return i;
			}
		} else {
			accepted = false;
			rejected = i;
			return rejected;
		}

		//rulesList
		bool rulesList = false;
		do {
			if (tokens.at(i)->GetType() == ID) {
				rulesList = true;
				i = RuleHandler(i);
				if (!accepted) {
					return i;
				}
			} else {
				return i;
			}
		} while (rulesList);

		std::cout << "ended in Rules()" << std::endl;
		exit(0);
		return -1;

	};
	int RuleHandler(int index){
		//not required
		//Rules-> headPredicate COLON_DASH predicate predicateList PERIOD
		//predicateList->COMMA predicate predicateList
		//headPredicate -> ID LEFT_PAREN ID idList RIGHT_PAREN //done
		//^ will use a big chunk of Schemes
		//predicate-> ID LEFT_PAREN parameter parameterList RIGHT_PAREN
		//parameter -> STRING | ID | expression
		//expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN
		//operator -> ADD | MULTIPLY
		//parameterList-> COMMA parameter parameterList
		//idList -> COMMA ID idList | lam

		int i = index;
		Rule* newRule = new Rule();
		Predicate* headPredicate = new Predicate();
		//headPredicate
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
			headPredicate->AddName(tokens.at(i)->GetString());
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		//idList
		if (tokens.at(i)->GetType() == ID) {
			//idList
			SimpleParameter* parameter1 = new SimpleParameter();
			parameter1->SetType(IDD);
			parameter1->SetParameter(tokens.at(i)->GetString());
			headPredicate->AddToBody(parameter1);
			i++;
			bool idList = false;
			do {
				if (tokens.at(i)->GetType() == COMMA) {
					idList = true;
					i++;
					if (tokens.at(i)->GetType() == ID) {
						SimpleParameter* parameter = new SimpleParameter();
						parameter->SetType(IDD);
						parameter->SetParameter(tokens.at(i)->GetString());
						headPredicate->AddToBody(parameter);
						i++;
						continue;
					} else {
						rejected = i;
						accepted = false;
						return rejected;
					}
				} else {
					idList = false;
				}
			} while (idList);

		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == RIGHT_PAREN) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		newRule->SetHeadPredicate(headPredicate);
		//end of headPredicate

		if (tokens.at(i)->GetType() == COLON_DASH) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		//predicate 1
		if (tokens.at(i)->GetType() == ID) {
			Predicate* passAlongPredicate = new Predicate();
			if (tokens.at(i)->GetType() == ID) {
				//start setting stuff obv
				//int startIndex = i;
				i = PredicateHandler(i, passAlongPredicate);
				newRule->AddPredicate(passAlongPredicate);

				if (!accepted) {
					return i;
				}
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}


		//predicateList
		bool predicateList = false;
		do {
			Predicate* passAlongPredicate = new Predicate();
			if (tokens.at(i)->GetType() == COMMA) {
				predicateList = true;
				i++;
				i = PredicateHandler(i, passAlongPredicate);
				newRule->AddPredicate(passAlongPredicate);
				if (!accepted) {
					return i;
				}
			} else {
				predicateList = false;
				break;
			}
		} while (predicateList);

		if (tokens.at(i)->GetType() == PERIOD) {
			datalog->AddRule(newRule);
			i++;
			return i;
		} else {
			accepted = false;
			rejected = i;
			return rejected;
		}

		std::cout << "Ended in Rule()" << std::endl;
		exit(0);
		return -1;
	};

	int Queries(int index){
		int i = index;
		i = Query(i);
		if (!accepted) {
			return i;
		}

		bool queryList = false;
		do {
			if (tokens.at(i)->GetType() == ID) {
				queryList = true;
				i = Query(i);
				if (!accepted) {
					return i;
				}
			} else {
				return i;
			}
		} while (queryList);

		std::cout << "ended in Queries()" << std::endl;
		exit(0);
		return -1;

	};
	int Query(int index){
		int i = index;
		Predicate* passAlongPredicate = new Predicate();

		i = PredicateHandler(i, passAlongPredicate);
		if (!accepted) {
			return i;
		}
		if (tokens.at(i)->GetType() == Q_MARK) {
			i++;
			datalog->AddQuery(passAlongPredicate);
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "Ended in Query()" <<std::endl;
		exit(0);
		return 0;
	};

	int PredicateHandler(int index, Predicate* &currentPredicate){
		int i = index;
		if (tokens.at(i)->GetType() ==ID) {
			currentPredicate->AddName(tokens.at(i)->GetString());
			i++;
		} else {
			//this shouldn't matter; for sake of data structures;
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i++;
			if (tokens.at(i)->GetType() == LEFT_PAREN) {
				ComplexParameter* parameter = new ComplexParameter();
				i = Expression(i, parameter);
				if (!accepted) {
					return i;
				}
				currentPredicate->AddToBody(parameter);
			} else {
				SimpleParameter* parameter = new SimpleParameter();
				i = ParameterHandler(i, parameter);
				if (!accepted) {
					return i;
				}
				currentPredicate->AddToBody(parameter);
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		bool parameterList = false;
		do {
			if (tokens.at(i)->GetType() == COMMA) {
				parameterList = true;
				i++;
			} else {
				parameterList = false;
				break;
			}
			if (tokens.at(i)->GetType() == LEFT_PAREN) {
				ComplexParameter* parameter = new ComplexParameter();
				i = Expression(i, parameter);
				if (!accepted) {
					return i;
				}
				currentPredicate->AddToBody(parameter);
			} else {
				SimpleParameter* parameter = new SimpleParameter();
				i = ParameterHandler(i, parameter);
				if (!accepted) {
					return i;
				}
				currentPredicate->AddToBody(parameter);
			}

		} while (parameterList);

		if (tokens.at(i)->GetType() == RIGHT_PAREN) {
			i++;
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "Ended in PredicateHandler()" << std::endl;
		exit(0);
		return 0;
	};
	int ParameterHandler(int index, SimpleParameter* &currentParameter){
		int i = index;
		if (tokens.at(i)->GetType() == ID) {
			currentParameter->SetType(IDD);
			currentParameter->SetParameter(tokens.at(i)->GetString());
			i++;
			return i;
		} else if (tokens.at(i)->GetType() == STRING) {
			currentParameter->SetType(STR);
			currentParameter->SetParameter(tokens.at(i)->GetString());
			i++;
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "end in Parameter()" << std::endl;
		exit(0);
		return 0;
	};
	int Expression(int index, ComplexParameter* &expression){
		//parameter -> STRING | ID | expression
		//expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN
		//operator -> ADD | MULTIPLY
		//this B is gonna be recursive
		int i = index;
		i++;
		//bool hasOperator = false;
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			ComplexParameter* parameter1 = new ComplexParameter();
			i = Expression(i, parameter1);
			if (!accepted) {
				return i;
			}
			expression->SetParameter1(parameter1);
		} else {
			SimpleParameter* parameter1 = new SimpleParameter();
			i = ParameterHandler(i, parameter1);
			if (!accepted) {
				return i;
			}
			expression->SetParameter1(parameter1);
		}

		if (tokens.at(i)->GetType() == ADD || tokens.at(i)->GetType() == MULTIPLY) {
			expression->SetOperator(tokens.at(i)->GetString());
			i++;
			//hasOperator = true;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			ComplexParameter* parameter2 = new ComplexParameter();
			i = Expression(i, parameter2);
			if (!accepted) {
				return i;
			}
			expression->SetParameter2(parameter2);
		} else {
			SimpleParameter* parameter2 = new SimpleParameter();
			i = ParameterHandler(i, parameter2);
			if (!accepted) {
				return i;
			}
			expression->SetParameter2(parameter2);
		}
		if (tokens.at(i)->GetType() == RIGHT_PAREN) {
			i++;
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "Ended in Expression()" << std::endl;
		exit(0);
		return 0;
	};
	std::string ParseExpression(int startIndex, int endIndex) {
		std::stringstream ss;
		for (int i = startIndex; i <= endIndex; i++) {
			ss << tokens.at(i)->GetString();
		}
		return ss.str();
	}

	void InputTokens(std::vector<Token*> inputTokens){
		for(unsigned int i = 0; i < inputTokens.size(); i++){
			if (inputTokens.at(i)->GetType() == COMMENT) {
				continue;
			} else {
				tokens.push_back(inputTokens.at(i));
			}
		}
		return;
	};

	void ReturnState(){
		if (accepted) {
			std::cout << "Success!" << std::endl;
			//std::cout << datalog->to_string();
		} else {
			std::cout << "Failure!" << std::endl;
			std::cout << tokens.at(rejected)->to_string() << std:: endl;
		}
	};

	DatalogProgram* GetDatalogProgram() {
		return datalog;
	}
};

#endif

//state of the code: probably just need to write a full predicate function to allow multiple predicates. that would help with the queries and rules classes.can't have predicates within predicates so it will be easier to implement than the expressions function. gosh, I don't know if any of this works. i need to go to bed.

//wow it's gotten worse. new plan:
/*
	re-implement all functions.
	be effing late. no way I'm getting this in in time. shooting for saturday or monday.
	functions in this .h:
		Run()
		Schemes()--runs Scheme() until end of SchemeList, until it hits Facts().
		Scheme()
		Facts()--runs Fact() until the cows go home
		Fact()
		Rules()
		Rule()
		Predicate()
		Parameter()
		Expression()--possibly the only one I won't need to rewrite
		Queries()
		Query();

	if stuff as is does not work, make an Expression.h class that has parameter and operator objects
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>

#include "../Lexer/Token.h"
#include "Rule.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Parameter.h"

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
		//ID LEFT_PAREN ID idList RIGHT_PAREN
		//idList-> COMMA ID idList
		int i = index;
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
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
			i++;
			while (accepted) {
				if (tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == ID) {
						i++;
						continue;
					} else {
						rejected = i;
						accepted = false;
						return rejected;
					}
				} else if (tokens.at(i)->GetType() == RIGHT_PAREN) {
					i++;
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
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
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
			//idList
			i++;
			while (accepted) {
				if (tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == STRING) {
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
			i = Rule(i);
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
				i = Rule(i);
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
	int Rule(int index){
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
		//headPredicate
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
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
			i++;
			bool idList = false;
			do {
				if (tokens.at(i)->GetType() == COMMA) {
					idList = true;
					i++;
					if (tokens.at(i)->GetType() == ID) {
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
			//start setting stuff obv
			i = Predicate(i);
			if (!accepted) {
				return i;
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		//predicateList
		bool predicateList = false;
		do {
			if (tokens.at(i)->GetType() == COMMA) {
				predicateList = true;
				i++;
				i = Predicate(i);
				if (!accepted) {
					return i;
				}
			} else {
				predicateList = false;
				break;
			}
		} while (predicateList);

		if (tokens.at(i)->GetType() == PERIOD) {
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

		i = Predicate(i);
		if (!accepted) {
			return i;
		}
		if (tokens.at(i)->GetType() == Q_MARK) {
			i++;
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

	int Predicate(int index){
		int i = index;
		if (tokens.at(i)->GetType() ==ID) {
			i++;
		} else {
			//this shouldn't matter; for sake of data structures;
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
		i = Parameter(i);
		if (!accepted) {
			return i;
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
			i = Parameter(i);
			if (!accepted) {
				return i;
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

		std::cout << "Ended in Predicate()" << std::endl;
		exit(0);
		return 0;
	};
	int Parameter(int index){
		int i = index;
		if (tokens.at(i)->GetType() == ID) {
			i++;
			return i;
		} else if (tokens.at(i)->GetType() == STRING) {
			i++;
			return i;
		} else if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i = Expression(i);
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
	int Expression(int index){
		//parameter -> STRING | ID | expression
		//expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN
		//operator -> ADD | MULTIPLY
		//this B is gonna be recursive
		int i = index;
		i++;
		//bool hasOperator = false;
		i = Parameter(i);
		if (!accepted) {
			return i;
		}
		if (tokens.at(i)->GetType() == ADD || tokens.at(i)->GetType() == MULTIPLY) {
			i++;
			//hasOperator = true;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		i = Parameter(i);
		if (!accepted) {
			return i;
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
		} else {
			std::cout << "Failure!" << std::endl;
			std::cout << tokens.at(rejected)->to_string() << std:: endl;
		}
	};
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
*/

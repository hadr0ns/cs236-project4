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
		//bool hasRules = false;
		//bool hasQueries = false;
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
		if (hasFacts == false) {
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
				i = Facts(i);
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
		} else if (tokens.at(i)->GetType() == RULES || tokens.at(i)->GetType() == QUERIES) {
			if (tokens.at(i)->GetType() == RULES) {
				i++;
				if (tokens.at(i)->GetType() == COLON) {
					i++;
				} else {
					rejected = i;
					accepted = false;
					ReturnState();
				}
				i = Rules(i);
				if (!accepted) {
					ReturnState();
					return;
				}
			} else if (tokens.at(i)->GetType() == QUERIES) {
				i++;
				if (tokens.at(i)->GetType() == COLON) {
					i++;
				} else {
					rejected = i;
					accepted = false;
					ReturnState();
				}
				i = Queries(i);
				if (!accepted) {
					ReturnState();
					return;
				}
			}
		} else if (tokens.at(i)->GetType() == ENDFILE) {
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
		//Facts()--runs Fact() until the cows go home
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
		return 0;
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
		//end of headPredicate

		if (tokens.at(i)->GetType() == COLON_DASH) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		//predicate(s)
		if (tokens.at(i)->GetType() == ID) {
			//start setting stuff obv
			i++;
			i = Predicate(i);
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		/* this shouldn't be necessary
		if (tokens.at(i)->GetType() == PERIOD) {
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		*/

		std::cout << "yo the rules fn is broken" << std::endl;
		exit(0);
		return -1;
	};

	int Parameter(int index){
		int i = index;
		if (tokens.at(i)->GetType() == ID || tokens.at(i)->GetType() == STRING || tokens.at(i)->GetType() == LEFT_PAREN) {
			//predicateList; structure builds a predicate and then checks for comma to continue;
			while (accepted) {
				if (tokens.at(i)->GetType() == ID) {
					i++;
				} else if (tokens.at(i)->GetType() == STRING) {
					i++;
				} else if (tokens.at(i)->GetType() == LEFT_PAREN) {
					i = Expression(i);
					if (!accepted) {
						rejected = i;
						return rejected;
					}
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
				if (tokens.at(i)->GetType() == RIGHT_PAREN || tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == COMMA) {
						i++;
						break;
					} else if (tokens.at(i)->GetType() == RIGHT_PAREN) {
						i++;
						if (tokens.at(i)->GetType() == COMMA) {
							i++; break;
						}
					}
					//can end at right paren; if it does not, it needs a comma and to loop with new parameters;
				}
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		return -1;
	};

	int Predicate(int index){
		int i = index;
		if (tokens.at(i)->GetType() == LEFT_PAREN) {
			i++;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		return -1;
	};

	int Expression(int index){
		//parameter -> STRING | ID | expression
		//expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN
		//operator -> ADD | MULTIPLY
		//this B is gonna be recursive
		int i = index;
		i++;
		if (tokens.at(i)->GetType() == STRING || tokens.at(i)->GetType() == ID || tokens.at(i)->GetType() == LEFT_PAREN) {
			bool hasOperator = false;
			while (accepted) {
				if (tokens.at(i)->GetType() == ID) {
					i++;
					if (hasOperator) {
						break;
					}
				} else if (tokens.at(i)->GetType() == STRING) {
					i++;
					if (hasOperator) {
						break;
					}
				} else if (tokens.at(i)->GetType() == LEFT_PAREN) {
					i = Expression(i);
					if (!accepted) {
						rejected = i;
						return rejected;
					}
					if (hasOperator) {
						break;
					}
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
				if (tokens.at(i)->GetType() == ADD || tokens.at(i)->GetType() == MULTIPLY) {
					i++;
					hasOperator = true;
				}
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}
		if (tokens.at(i)->GetType() == RIGHT_PAREN) {
			i++;
			return i;
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "expressions is broken" << std::endl;
		exit(0);
		return 0;
	};

	int Queries(int index){
		return 0;
	};
	int Query(int index){
		//re-implement using Predicate and Parameter function above
		//colon query querylist EOF
		//query->predicate Q_MARK
		//predicate-> ID LEFT_PAREN parameter parameterList RIGHT_PAREN
		//parameter -> STRING | ID | expression
		//queryList->query queryList;
		int i = index;

		//can I adapt the rules code here, just with end Q_Mark?
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
		if (tokens.at(i)->GetType() == ID || tokens.at(i)->GetType() == STRING || tokens.at(i)->GetType() == LEFT_PAREN) {
			//predicateList; structure builds a predicate and then checks for comma to continue;
			while (accepted) {
				if (tokens.at(i)->GetType() == ID) {
					i++;
				} else if (tokens.at(i)->GetType() == STRING) {
					i++;
				} else if (tokens.at(i)->GetType() == LEFT_PAREN) {
					i = Expression(i);
					if (!accepted) {
						rejected = i;
						return rejected;
					}
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
				if (tokens.at(i)->GetType() == RIGHT_PAREN || tokens.at(i)->GetType() == COMMA) {
					i++;
					if (tokens.at(i)->GetType() == COMMA) {
						i++;
						break;
					} else if (tokens.at(i)->GetType() == RIGHT_PAREN) {
						i++;
						if (tokens.at(i)->GetType() == COMMA) {
							i++; break;
						}
					}
					//can end at right paren; if it does not, it needs a comma and to loop with new parameters;
				}
				//it shouldn't make it down here if the above code works as expected; thus, it either reaches something wrong or it is the end of the list and it returns.
				if (tokens.at(i)->GetType() == Q_MARK) {
					i++;
					break;
				} else {
					rejected = i;
					accepted = false;
					return rejected;
				}
			}
			if (tokens.at(i)->GetType() == ID) {
				i = Queries(i);
				if (!accepted) {
					return rejected;
				} else {
					return i;
				}
			} else {
				return i;
			}
		} else {
			rejected = i;
			accepted = false;
			return rejected;
		}

		std::cout << "Queries is busted" <<std::endl;
		exit(0);
		return 0;
	};

	void InputTokens(std::vector<Token*> inputTokens){
		for(unsigned int i = 0; i < inputTokens.size(); i++){
			if (inputTokens.at(i)->GetType() == COMMENT) {
				continue;
			} else {
				tokens.at(i) = inputTokens.at(i);
			}
	}
	return;};

	void ReturnState(){
		if (accepted) {
			std::cout << "Success!" << std::endl;
		} else {
			std::cout << "Failure!" << std::endl;
			std::cout << tokens.at(rejected)->to_string();
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

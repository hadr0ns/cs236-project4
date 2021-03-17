#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <sstream>

enum TokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, ENDFILE, ERROR };
//add in other types here
//TokenType myNewType = COMMA;
std::string tokenTypeToString(TokenType type) {
	switch (type) {
		case COMMA: return "COMMA"; break;
		case PERIOD: return "PERIOD"; break;
		case Q_MARK: return "Q_MARK"; break;
		case LEFT_PAREN: return "LEFT_PAREN"; break;
		case RIGHT_PAREN: return "RIGHT_PAREN"; break;
		case COLON: return "COLON"; break;
		case COLON_DASH: return "COLON_DASH"; break;
		case MULTIPLY: return "MULTIPLY"; break;
		case ADD: return "ADD"; break;
		case SCHEMES: return "SCHEMES"; break;
		case FACTS: return "FACTS"; break;
		case RULES: return "RULES"; break;
		case QUERIES: return "QUERIES"; break;
		case ID: return "ID"; break;
		case STRING: return "STRING"; break;
		case COMMENT: return "COMMENT";
		case UNDEFINED: return "UNDEFINED"; break;
		case ENDFILE: return "EOF"; break;
		case ERROR: return "ERROR"; break;
		//case else: return "else error"; break;
	}
	return "ERROR";
}

class Token {
private:
	TokenType type;
	std::string associatedString;
	int lineNumber;
public:
	std::string to_string() {
		std::string tokenString;
		std::string tokenTypeString = tokenTypeToString(type);
		std::stringstream ss;
		ss << lineNumber;
		std::string lineNumberString = ss.str();
		tokenString = "(" + tokenTypeString + ",\"" + associatedString + "\"," + lineNumberString + ")";
		//std::cout << tokenString << std::endl;
		return tokenString;
	}
	void SetString(std::string input){
		associatedString = input;
	}
	void SetLineNumber(int line){
		lineNumber = line;
	}
	void SetType(TokenType newType) {
		type = newType;
	}
	TokenType GetType() {
		return type;
	}
	std::string GetString() {
		return associatedString;
	}
	int GetLineNumber() {
		return lineNumber;
	}



};

#endif /* end of include guard: TOKEN_H */

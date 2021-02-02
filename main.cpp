#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Lexer.h"


int main(int argc, char *argv[]) {
	std::string filename = argv[1];

	std::ifstream iff;
	iff.open(filename);
	if (iff.is_open()){
		//std::cout << "opened file " << filename << " successfully!" << std::endl;
	} else {
		std::cout << "could not open file " << filename << std::endl;
		exit (0);
	}
	std::string fileContents;
	std::stringstream ss;
	ss << iff.rdbuf();
	fileContents = ss.str();
	//std::cout << fileContents << std::endl;
	iff.close();

	Lexer* lexer = new Lexer();
	lexer->Run(fileContents);

	return 0;
}

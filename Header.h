#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
#include <sstream>

class Header {
private:
	std::vector<std::string> attributes;
	std::string name;

public:
	Header() {};
	void AddAttribute(std::string attribute) {
		attributes.push_back(attribute);
	}
	void SetName(std::string input) {
		name = input;
	}
	std::string GetName() {
		return name;
	}
	std::string to_string() {
		std::stringstream ss;
		ss<<"\nHeader: " << std::endl;
		for (unsigned int i = 0; i < attributes.size(); i++) {
			ss<<i<<": "<<attributes.at(i)<<std::endl;
		}
		return ss.str();
	}
	std::string GetAttribute(int index) {
		return attributes.at(index);
	}
	int GetSize() {
		return attributes.size();
	}
};

#endif

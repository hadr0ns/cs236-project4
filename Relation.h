#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <vector>
#include <set>
#include <list>
#include <sstream>
#include <pair>

#include "Tuple.h"
#include "Header.h"

class Relation {
private:
	std::set<Tuple> tuples;
	std::string name;
	Header* header;

public:
	Relation(){};
	//should these be void? or a vector of tuples lol--ans:modifying an existing relation to match
	Relation* Select(int index, std::string value) {
		//std::cout << this->to_string()<<std::endl;
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			//std::cout<<elem->to_string()<<std::endl;

			if (elem.GetColumn(index) == value) {
				returnRelation->AddTuple(elem);
			}
		}
		//PrintRemainingTuples();
		return returnRelation;
	};
	Relation* Select(int index1, int index2) {
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			if (elem.GetColumn(index1) == elem.GetColumn(index2)) {
				returnRelation->AddTuple(elem);
			}
		}
		//PrintRemainingTuples();
		return returnRelation;
	};
	Relation* Project(std::vector<int> indices) {
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			Tuple newTuple;
			for (unsigned int i = 0; i < indices.size(); i++) {
				newTuple.AddValue(elem.GetColumn(indices.at(i)));
			}
			returnRelation->AddTuple(newTuple);
		}
		return returnRelation;
	};
	Relation* Rename(std::vector<std::string> attributes){
		Relation* returnRelation = new Relation();
		for (auto elem : tuples) {
			returnRelation->AddTuple(elem);
		}
		Header* header = new Header();
		for (auto elem : attributes) {
			header->AddAttribute(elem);
		}
		returnRelation->SetHeader(header);
		return returnRelation;

	};
	Relation* Join(Relation* joinRelation) {
		Relation* returnRelation = new Relation();
		Header* header2 = joinRelation->GetHeader();
		//join headers
		Header* newHeader = CombineHeaders(header, header2);
		returnRelation->SetHeader(newHeader);
		//get pairs of where the tuples should match here
		std::vector<std::pair<int, int>> shouldMatch;
		for (unsigned int i = 0; i < header->GetAttributes().size(); i++) {
			for (unsigned int j = 0; j < header2->GetAttributes().size(); j++) {
				if (header->GetAttribute(i) == header2->GetAttribute(j)) {
					shouldMatch.push_back(std::make_pair(i,j));
				}
			}
		}
		for (auto tuple1 : tuples) {
			for (auto tuple2 : joinRelation->GetTuples()) {
				if (isJoinable(tuple1, tuple2, shouldMatch)) {
					Tuple newTuple = CombineTuples(tuple1, tuple2, shouldMatch);
					returnRelation->AddTuple(newTuple);
				}
			}
		}
		return returnRelation;
	};
	bool isJoinable(Tuple tuple1, Tuple tuple2, std::vector<std::pair<int, int>> shouldMatch) {
		bool retVal = true;
		for (unsigned int i = 0; i < shouldMatch.size(); i++) {
			if (tuple1->GetColumn(shouldMatch.at(i).first) != tuple2->GetColumn(shouldMatch.at(i).second)) {
				return false;
			}
			//else it just stays true if they all match and then returns true at the end
		}
		return retVal;
	};
	Tuple CombineTuples(Tuple tuple1, Tuple tuple2, std::vector<std::pair<int, int>> shouldMatch) {
		std::vector<std::string> tuple1values = tuple1->GetValues();
		std::vector<std::string> tuple2values = tuple2->GetValues();
		Tuple newTuple;
		for (unsigned int i = 0; i < tuple1values.size(); i++) {
			newTuple->AddValue(tuple1values.at(i));
		}
		for (unsigned int i = 0; i < tuple2values.size();i++) {
			bool isUnique = true;
			for (unsigned int j = 0; j < tuple1values.size(); j++) {
				if (tuple2values.at(i)==tuple1values.at(j)) {
					isUnique = false;
				}
			}
			if (isUnique == true) {
				newTuple->AddValue(tuple2values.at(i));
			}
		}
		return newTuple;
	};
	Header* CombineHeaders(Header* header1, Header* header2) {
		Header* newHeader = new Header();
		std::vector<std::string> header1attributes = header1->GetAttributes();
		std::vector<std::string> header2attributes = header2->GetAttributes();
		for (unsigned int i = 0; i < header1attributes.size(); i++) {
			newHeader->AddAttribute(header1attributes.at(i));
		}
		for (unsigned int i = 0; i < header2attributes.size();i++) {
			bool isUnique = true;
			for (unsigned int j = 0; j < header1attributes.size(); j++) {
				if (header2attributes.at(i)==header1attributes.at(j)) {
					isUnique = false;
				}
			}
			if (isUnique == true) {
				newHeader->AddAttribute(header2attributes.at(i));
			}
		}
		return newHeader;
	};
	Header* GetHeader() {
		return header;
	}
	void SetName(std::string string) {
		name = string;
	}
	void SetHeader(Header* input) {
		header = input;
	}
	void AddTuple(Tuple tuple) {
		tuples.insert(tuple);
	}
	std::set<Tuple> GetTuples() {
		return tuples;
	}
	std::string GetName() {
		return name;
	}
 	std::string to_string() {
		std::stringstream ss;
		ss<<header->to_string();
		ss<<"Tuples: ";
		int i = 0;
		for (auto elem : tuples) {
			ss<<i<<": "<< elem.to_string();
			i++;
		}
		return ss.str();
	}
	void PrintRemainingTuples() {
		std::cout << "Remaining:" <<std::endl;
		for (auto elem : tuples) {
			std::cout << elem.to_string()<<std::endl;
		}
	}
	std::string evaluated_to_string() {
		std::stringstream ss;
		if (is_empty()) {
			ss<<"No";
		} else {
			ss << "Yes(" << tuples.size() << ")";
			//fill in the rest here
			if (header->GetSize() > 0) {
				for (auto elem : tuples) {
					ss << "\n  ";
					for (int i = 0; i < header->GetSize(); i++) {
						if (i > 0) {
							ss <<", ";
						}
						ss << header->GetAttribute(i) << "="<< elem.GetColumn(i);
					}
				}
			}


		}
		return ss.str();
	}
	bool is_empty() {
		return tuples.empty();
	}

};

#endif

//std::set<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); it++)

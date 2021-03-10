#pragma once
#include <string>

using namespace std;

class Concept {
protected:
	string name;
	string description;
public:
	Concept() {};
	Concept(string name);
	Concept(string name, string description);

	string getName() { return name; };
	string getDescription() { return description; };
};

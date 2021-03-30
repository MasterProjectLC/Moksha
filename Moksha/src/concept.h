#pragma once
#include <string>

using namespace std;

class Concept {
protected:
	string name;
	string codename;
	string description;
public:
	Concept() {};
	Concept(string name, string description);
	Concept(string name, string codename, string description);

	string getName() { return name; };
	string getCodename() { return codename; };
	string getDescription() { return description; };
};

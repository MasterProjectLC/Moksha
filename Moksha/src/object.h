#pragma once
#include <vector>
#include "dictionary.h"
#include "IObservable.h"

using namespace std;

class Object : public IObservable {
private:
	Dictionary<vector<string>> dict;
	string name;
	string codename;
	string user;
	vector<string> validActions;
	bool visible;

public:
	Object() {};
	Object(Dictionary<vector<string>> dict);
	enum { obter };

	// Getters
	string getName() { return name; }
	string getCodename() { return codename; }
	string getUser() { return user; }
	bool isVisible() { return visible; }

	vector<string> getResponses(string action);
	void takeAction(string prompt, string user);
};
#pragma once
#include <vector>
#include "dictionary.h"
#include "IObservable.h"

using namespace std;

class Objeto : public IObservable {
private:
	Dictionary<vector<string>> dict;
	string name;
	string user;
	vector<string> validActions;

public:
	Objeto() {};
	Objeto(Dictionary<vector<string>> dict);
	enum { obter };

	// Getters
	string getName() { return name; }
	string getUser() { return user; }

	vector<string> getResponses(string action);
	void takeAction(string prompt, string user);
};
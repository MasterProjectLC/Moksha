#pragma once
#include <vector>
#include "dictionary.h"
#include "IObservable.h"

using namespace std;

class Objeto : public IObservable {
private:
	Dictionary<vector<string>> dict;
	string name;
	vector<string> validActions;

public:
	Objeto() {};
	Objeto(Dictionary<vector<string>> dict);
	enum { obter };

	// Getters
	string getName() { return name; }

	//vector<vector<string>> getActions() { return dict.; };
	vector<string> getResponses(string action);

	void takeAction(string prompt);
};
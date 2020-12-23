#pragma once
#include <vector>
#include "IObservable.h"

using namespace std;

class Objeto : public IObservable {
private:
	string name;

	vector<string> validActions;
	vector<vector<string>> actions;
	vector<vector<string>> responses;

public:
	Objeto() {};
	Objeto(string name, vector<string> validActions, vector<vector<string>> actions, vector<vector<string>> responses);
	enum { obter };

	// Getters
	string getName() { return name; }

	vector<vector<string>> getActions() { return actions; };
	vector<string> getResponses(string action);

	void takeAction(string prompt);
};
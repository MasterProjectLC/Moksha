#pragma once
#include <vector>

using namespace std;

class Objeto {
private:
	string name;

	vector<string> validActions;
	vector<vector<string>> actions;
	vector<vector<string>> responses;

public:
	Objeto() {};

	Objeto(string name, vector<string> validActions, vector<vector<string>> actions, vector<vector<string>> responses);

	// Getters
	string getName() { return name; }

	vector<vector<string>> getActions() { return actions; };
	vector<string> getResponses(string action);
};
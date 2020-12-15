#include "objeto.h"

Objeto::Objeto(string name, vector<string> validActions, vector<vector<string>> actions, vector<vector<string>> responses) {
	this->name = name;
	this->validActions = validActions;
	this->actions = actions;
	this->responses = responses;
}

vector<string> Objeto::getResponses(string action) {
	for (int i = 0; i < getActions().size(); i++)
		// Is one of the actions here compatible?
		for (int j = 0; j < getActions()[i].size(); j++)
			if (getActions()[i][j].compare(action) == 0)
				return responses[i];

	vector<string> a;
	return a;
}
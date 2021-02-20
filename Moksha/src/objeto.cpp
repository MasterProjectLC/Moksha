#include "objeto.h"

Objeto::Objeto(Dictionary<vector<string>> dict) {
	this->dict = dict;
	this->name = dict.getValue("name");
	this->validActions = dict.getValues("actions");
}

vector<string> Objeto::getResponses(string action) {
	return dict.getValues(action);
}

void Objeto::takeAction(string prompt, string user) {
	for (int i = 0; i < validActions.size(); i++) {
		if (validActions[i].compare(prompt) == 0) {
			this->user = user;
			if (prompt == "obtain" || prompt == "take")
				notify(obter);
		}
	}
}
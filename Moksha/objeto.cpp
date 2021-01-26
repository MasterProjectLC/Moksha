#include "objeto.h"

Objeto::Objeto(Dictionary<vector<string>> dict) {
	this->dict = dict;
	this->name = dict.getValue("nome");
	this->validActions = dict.getValues("acoes");
}

vector<string> Objeto::getResponses(string action) {
	return dict.getValues(action);
}

void Objeto::takeAction(string prompt) {
	for (int i = 0; i < validActions.size(); i++) {
		if (validActions[i].compare(prompt) == 0) {
			if (prompt == "obter")
				notify(obter);
		}
	}
}
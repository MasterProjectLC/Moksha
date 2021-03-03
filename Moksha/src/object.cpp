#include "object.h"

Object::Object(Dictionary<vector<string>> dict) {
	this->dict = dict;
	this->name = dict.getValue("name");
	this->visible = (dict.getValue("visible") == "true");
	this->validActions = dict.getValues("actions");
}

vector<string> Object::getResponses(string action) {
	return dict.getValues(action);
}

void Object::takeAction(string prompt, string user) {
	for (int i = 0; i < validActions.size(); i++) {
		if (validActions[i].compare(prompt) == 0) {
			this->user = user;
			if (prompt == "obtain" || prompt == "take")
				notify(obter);
		}
	}
}
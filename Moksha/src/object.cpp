#include "object.h"

Object::Object(Dictionary<vector<string>> dict) {
	if (actionName.empty()) {
		actionName.insert(pair<string, int>("obtain", obter));
		actionName.insert(pair<string, int>("move", mover));
		actionName.insert(pair<string, int>("peer", espiar));
		actionName.insert(pair<string, int>("open", abrir));
		actionName.insert(pair<string, int>("time", tempo));
	}

	this->dict = dict;
	this->name = dict.getValue("name");
	if (dict.hasKey("codename"))
		this->codename = dict.getValue("codename");
	else
		this->codename = dict.getValue("name");
	this->visible = (dict.getValue("visible") == "true");
	this->validActions = dict.getValues("actions");
}

string Object::getResponse(string action) {
	if (dict.hasKey(action))
		return dict.getValues(action)[0];
	else
		return string("");
}

void Object::takeAction(string prompt, string user) {
	for (int i = 0; i < validActions.size(); i++) {
		if (validActions[i].compare(prompt) == 0) {
			this->user = user;
			args = dict.getValues(prompt);
			if (args.size() > 1 && actionName.count(args[1]) > 0) {
				int action = actionName[args[1]];
				for (int i = 0; i < 2; i++, args.erase(args.begin()));
				notify(action);
			}
		}
	}
}
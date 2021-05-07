#include "object.h"

Object::Object(Dictionary<vector<string>> dict) {
	if (actionName.empty()) {
		actionName.insert(pair<string, int>("obtain", pegar));
		actionName.insert(pair<string, int>("move", mover));
		actionName.insert(pair<string, int>("peer", espiar));
		actionName.insert(pair<string, int>("open", abrir));
		actionName.insert(pair<string, int>("time", tempo));
		actionName.insert(pair<string, int>("reveal", revelar));
		actionName.insert(pair<string, int>("transform", transformar));
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

int Object::returnAction(string prompt) {
	for (int i = 0; i < validActions.size(); i++)
		if (validActions[i].compare(prompt) == 0)
			goto SCRIPT;
	return -1;

	SCRIPT:
	args = dict.getValues(prompt);
	if (args.size() <= 1)
		return -1;
	string action = args[1];
	for (int j = 0; j < 2; j++, args.erase(args.begin()));
	return actionName[action];
}

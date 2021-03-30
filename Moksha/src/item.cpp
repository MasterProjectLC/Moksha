#include "item.h"

Item::Item(string name, string codename, string description, set<string> actionNames) : Concept(name, codename, description) {
	this->actionNames = actionNames;

	for (set<string>::iterator it = actionNames.begin(); it != actionNames.end(); it++) {
		string s = *it;
		if (s == "atacar")
			actions.insert(atacar);
	}
}

bool Item::isActionValid(string action) {
	bool retorno = actionNames.count(action) > 0;
	return retorno;
}

bool Item::isActionValid(int action) {
	return actions.count(action) > 0;
}
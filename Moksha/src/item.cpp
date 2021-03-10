#include "item.h"

Item::Item(string name, string description, set<string> actionNames) : Concept(name, description) {
	this->actionNames = actionNames;

	for (set<string>::iterator it = actionNames.begin(); it != actionNames.end(); it++) {
		string s = *it;
		if (s == "atacar")
			actions.insert(atacar);
	}
}

bool Item::isActionValid(string action) {
	return actionNames.count(action) > 0;
}

bool Item::isActionValid(int action) {
	return actions.count(action) > 0;
}
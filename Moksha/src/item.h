#pragma once
#include <vector>
#include <set>
#include "concept.h"
#include "actionEnum.h"

using namespace std;

class Item : public Concept {
private:
	set<string> actionNames;
	set<int> actions;

public:
	Item(string name) : Item(name, "", set<string>()) {};
	Item(string name, set<string> actions) : Item(name, "", actions) {};
	Item(string name, string description, set<string> actions);

	bool isActionValid(string action);
	bool isActionValid(int action);
};

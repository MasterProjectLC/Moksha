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
	Item(string name, string codename, string description, set<string> actions);

	bool isActionValid(string action);
	bool isActionValid(int action);
};

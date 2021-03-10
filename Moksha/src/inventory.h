#pragma once
#include <vector>
#include "item.h"

using namespace std;

class Inventory {
private:
	vector<Concept> concepts;
	vector<Item> items;

public:
	vector<Concept> getConcepts() { return concepts; };
	vector<Item> getItems() { return items; };
	void addConcept(string name);
	void addItem(string name, set<string> actions);
	void addItem(string name, string description, set<string> actions);

	bool hasItem(string name);
	bool hasConcept(string name);
};

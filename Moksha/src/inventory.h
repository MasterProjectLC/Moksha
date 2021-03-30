#pragma once
#include <vector>
#include "item.h"

using namespace std;

class Inventory {
private:
	vector<Concept*> concepts;
	vector<Concept*> rumors;
	vector<Item*> items;

public:
	~Inventory();

	vector<Concept*> getConcepts() { return concepts; };
	vector<Concept*> getRumors() { return rumors; };
	vector<Item*> getItems() { return items; };
	void addConcept(string name);
	void addRumor(string name);
	void addItem(string name, string codename, string description, set<string> actions);
	void removeItem(string name);

	Item* getItem(string name);
	bool hasItem(string name);
	bool hasRumor(string name);
	bool hasConcept(string name);
};

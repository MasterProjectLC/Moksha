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
	void clear();
	void clearItems();

	vector<Concept*> getConcepts() { return concepts; };
	vector<Concept*> getRumors() { return rumors; };
	vector<Item*> getItems() { return items; };
	void addAbstract(string name, string codename, string description, char type);
	void addItem(string name, string codename, string description, set<string> actions);
	void removeItem(string name);

	Item* getItem(string name);
	Concept* getInfo(string codename);
	Concept* getInfoByName(string name);
	bool hasItem(string name);
	bool hasRumor(string name);
	bool hasConcept(string name);
};

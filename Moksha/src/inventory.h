#pragma once
#include <vector>
#include "item.h"

using namespace std;

class Inventory {
private:
	vector<Conceito> inventory;
	vector<Item> items;

public:
	vector<Conceito> getInventory() { return inventory; };
	vector<Item> getItems() { return items; };
	void addConcept(string name);
	void addItem(string name, set<string> actions);
	void addItem(string name, string description, set<string> actions);

	bool hasItem(string name);
	bool hasConcept(string name);
};

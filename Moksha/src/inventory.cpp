#include "inventory.h"

void Inventory::addConcept(string name) {
	concepts.push_back(Concept(name));
}

void Inventory::addRumor(string name) {
	rumors.push_back(Concept(name));
}

void Inventory::addItem(string name, set<string> acoes) {
	items.push_back(Item(name, acoes));
}

void Inventory::addItem(string name, string description, set<string> actions) {
	items.push_back(Item(name, description, actions));
}

bool Inventory::hasRumor(string name) {
	for (int i = 0; i < rumors.size(); i++)
		if (rumors[i].getName() == name)
			return true;
	return false;
}


bool Inventory::hasConcept(string name) {
	for (int i = 0; i < concepts.size(); i++)
		if (concepts[i].getName() == name)
			return true;
	return false;
}

bool Inventory::hasItem(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i].getName() == name)
			return true;
	return false;
}
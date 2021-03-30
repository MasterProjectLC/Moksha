#include "inventory.h"

Inventory::~Inventory() {
	for (int i = 0; i < concepts.size(); i++)
		delete concepts[i];

	for (int i = 0; i < rumors.size(); i++)
		delete rumors[i];

	for (int i = 0; i < items.size(); i++)
		delete items[i];
}

void Inventory::addConcept(string name) {
	concepts.push_back(new Concept(name, ""));
}

void Inventory::addRumor(string name) {
	rumors.push_back(new Concept(name, ""));
}

void Inventory::addItem(string name, string codename, string description, set<string> actions) {
	items.push_back(new Item(name, codename, description, actions));
}

void Inventory::removeItem(string codename) {
	for (vector<Item*>::iterator it = items.begin(); it != items.end(); it++)
		if ((*it)->getCodename() == codename) {
			delete *it;
			items.erase(it);
			break;
		}
}

bool Inventory::hasRumor(string name) {
	for (int i = 0; i < rumors.size(); i++)
		if (rumors[i]->getName() == name)
			return true;
	return false;
}


bool Inventory::hasConcept(string name) {
	for (int i = 0; i < concepts.size(); i++)
		if (concepts[i]->getName() == name)
			return true;
	return false;
}

bool Inventory::hasItem(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getName() == name)
			return true;
	return false;
}

Item* Inventory::getItem(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getName() == name)
			return items[i];
	return NULL;
}
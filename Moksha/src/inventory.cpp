#include "inventory.h"

Inventory::~Inventory() {
	clear();
}

void Inventory::clear() {
	for (int i = 0; i < concepts.size(); i++)
		delete concepts[i];

	for (int i = 0; i < rumors.size(); i++)
		delete rumors[i];

	for (int i = 0; i < items.size(); i++)
		delete items[i];
}

void Inventory::clearItems() {
	for (int i = 0; i < items.size(); i++)
		delete items[i];
}


void Inventory::addAbstract(string name, string codename, string description, char type) {
	if (type == 'c')
		concepts.push_back(new Concept(name, codename, description));
	else if (type == 'r')
		rumors.push_back(new Concept(name, codename, description));
	else
		throw invalid_argument("Invalid type");

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
		if (rumors[i]->getName() == name || rumors[i]->getCodename() == name)
			return true;
	return false;
}


bool Inventory::hasConcept(string name) {
	for (int i = 0; i < concepts.size(); i++)
		if (concepts[i]->getName() == name || concepts[i]->getCodename() == name)
			return true;
	return false;
}

bool Inventory::hasItem(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getName() == name || items[i]->getCodename() == name)
			return true;
	return false;
}

Item* Inventory::getItem(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getName() == name || items[i]->getCodename() == name)
			return items[i];
	return NULL;
}

Concept* Inventory::getInfo(string codename) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getCodename() == codename)
			return items[i];
	for (int i = 0; i < rumors.size(); i++)
		if (rumors[i]->getCodename() == codename)
			return rumors[i];
	for (int i = 0; i < concepts.size(); i++)
		if (concepts[i]->getCodename() == codename)
			return items[i];
	return NULL;
}


Concept* Inventory::getInfoByName(string name) {
	for (int i = 0; i < items.size(); i++)
		if (items[i]->getName() == name)
			return items[i];
	for (int i = 0; i < rumors.size(); i++)
		if (rumors[i]->getName() == name)
			return rumors[i];
	for (int i = 0; i < concepts.size(); i++)
		if (concepts[i]->getName() == name)
			return concepts[i];
	return NULL;
}
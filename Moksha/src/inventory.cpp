#include "inventory.h"

void Inventory::addConcept(string nome) {
	concepts.push_back(Concept(nome));
}

void Inventory::addItem(string nome, set<string> acoes) {
	items.push_back(Item(nome, acoes));
}

void Inventory::addItem(string nome, string descricao, set<string> acoes) {
	items.push_back(Item(nome, descricao, acoes));
}

bool Inventory::hasConcept(string name) {
	for (int i = 0; i < concepts.size(); i++) {
		if (concepts[i].getName() == name) {
			return true;
		}
	}

	return false;
}

bool Inventory::hasItem(string nome) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].getName() == nome) {
			return true;
		}
	}

	return false;
}
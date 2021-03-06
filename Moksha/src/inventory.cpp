#include "inventory.h"

void Inventory::addConcept(string nome) {
	inventory.push_back(Conceito(nome));
}

void Inventory::addItem(string nome, set<string> acoes) {
	items.push_back(Item(nome, acoes));
}

void Inventory::addItem(string nome, string descricao, set<string> acoes) {
	items.push_back(Item(nome, descricao, acoes));
}

bool Inventory::hasConcept(string nome) {
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i].getNome() == nome) {
			return true;
		}
	}

	return false;
}

bool Inventory::hasItem(string nome) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].getNome() == nome) {
			return true;
		}
	}

	return false;
}
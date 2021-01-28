#include "inventario.h"

void Inventario::addConceito(string nome) {
	inventario.push_back(Conceito(nome));
}

void Inventario::addItem(string nome, set<string> acoes) {
	itens.push_back(Item(nome, acoes));
}

void Inventario::addItem(string nome, string descricao, set<string> acoes) {
	itens.push_back(Item(nome, descricao, acoes));
}

bool Inventario::temConceito(string nome) {
	for (int i = 0; i < inventario.size(); i++) {
		if (inventario[i].getNome() == nome) {
			return true;
		}
	}

	return false;
}

bool Inventario::temItem(string nome) {
	for (int i = 0; i < itens.size(); i++) {
		if (itens[i].getNome() == nome) {
			return true;
		}
	}

	return false;
}
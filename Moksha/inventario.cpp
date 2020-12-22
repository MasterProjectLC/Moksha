#include "inventario.h"

void Inventario::addConceito(string nome) {
	inventario.push_back(Conceito(nome));
}

void Inventario::addItem(string nome) {
	itens.push_back(Item(nome));
}
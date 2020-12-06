#include "inventario.h"

vector<Conceito> Inventario::getInventario() {
	return inventario;
}

void Inventario::addConceito(string nome) {
	inventario.push_back(Conceito(nome));
}


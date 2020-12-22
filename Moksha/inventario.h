#pragma once
#include <vector>
#include "item.h"

using namespace std;

class Inventario {
private:
	vector<Conceito> inventario;
	vector<Item> itens;

public:
	vector<Conceito> getInventario() { return inventario; };
	vector<Item> getItens() { return itens; };
	void addConceito(string nome);
	void addItem(string nome);
};

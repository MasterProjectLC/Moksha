#pragma once
#include "inventario.h"

using namespace std;

class Personagem {
private:
	vector<string> acoesBasicas{"obter", "mover", "examinar", "ver", "tocar"};

	Inventario inventario;

public:
	vector<Item> getInventario() { return inventario.getItens(); };
	void addConceito(string nome) { inventario.addConceito(nome); };
	void addItem(string nome) { inventario.addItem(nome); };

	bool isAcaoValida(string acao);
};

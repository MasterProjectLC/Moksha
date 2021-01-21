#pragma once
#include "sala.h"
#include "inventario.h"
#include "IObservable.h"

using namespace std;

class Personagem: public IObservable {
protected:
	vector<string> acoesBasicas{"obter", "mover", "examinar", "ver", "tocar"};
	string notifyText;
	
	Sala *salaAtual;
	Inventario inventario;

public:
	vector<Item> getInventario() { return inventario.getItens(); };
	void addConceito(string nome) { inventario.addConceito(nome); };
	void addItem(string nome) { inventario.addItem(nome); };

	bool isAcaoValida(string acao);

	void setSalaAtual(Sala *sala) { salaAtual = sala; }
	Sala* getSalaAtual() { return salaAtual; }

	void takeAction() {};

	enum { imprimir, mover };
	void printText(string str);
	void move(string str);
	void move(Sala sala);
	string getNotifyText() { return notifyText; };
};

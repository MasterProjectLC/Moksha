#pragma once
#include "sala.h"
#include "inventario.h"
#include "IObservable.h"

using namespace std;

class Personagem: public IObservable {
protected:
	string nome;
	string notifyText;
	vector<string> notifyTargets;
	
	Sala *salaAtual;
	Inventario inventario;

public:
	string getNome() { return nome; }

	vector<Item> getInventario() { return inventario.getItens(); }
	void addConceito(string nome) { inventario.addConceito(nome); }
	void addItem(string nome) { inventario.addItem(nome); }

	void setSalaAtual(Sala *sala);
	Sala* getSalaAtual();

	void takeAction() {}

	enum { imprimir, mover, mencionar, falar };
	void printText(string str);
	void move(string str);
	void move(Sala sala);
	void mention(string obj, vector<string> receivers);
	void say(string topico, string str, vector<string> receivers);

	string getNotifyText() { return notifyText; }
	vector<string> getNotifyTargets() { return notifyTargets; }

	virtual void executarReacao(string topico, string frase, string remetente) {};
	virtual void verSala(vector<string> pessoasNaSala) {};
	virtual void verPessoaEntrando(string nomePessoa) {};
};

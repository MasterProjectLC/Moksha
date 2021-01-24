#pragma once
#include "Jenna.h"
#include "jogador.h"
#include "IObservable.h"
#include "IObserver.h"
#include "mapa.h"
#include "fileManager.h"

using namespace std;

class Jogo : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 500;

	string texto;

	//vector<Objeto> objetos;
	Jogador jogador;
	Jenna jenna;
	vector<Personagem*> personagens;

	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;

	void addItem(string item);

	void gerarMapa();
	Sala* moverSala(Sala *salaOrigem, string salaDestino);
	Mapa mapa;
	vector<string> getPessoasNaSala(Sala* sala);

	class No;

public:
	Jogo();

	void update(int id) override;
	void advanceTime();

	enum { obter, imprimir };

	Sala* getSalaAtual() { return jogador.getSalaAtual(); }
	void receberArgs(vector<string> args);

	vector<Item> getInventario();

	void imprimirTexto(string texto);
	string getTexto() { return texto; }
};
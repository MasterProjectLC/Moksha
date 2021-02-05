#pragma once
#include "Jenna.h"
#include "jogador.h"
#include "IObservable.h"
#include "IObserver.h"
#include "mapa.h"
#include "fileManager.h"
#include "libs/pugixml/src/pugixml.hpp"
#include "saveModule.h"

using namespace std;
using namespace pugi;

class Jogo : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 500;

	int time;
	int loop;

	string texto;

	//vector<Objeto> objetos;
	Jogador jogador;
	Jenna jenna;
	vector<Personagem*> personagens;
	vector<NPC*> npcs;

	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;

	Sala* moverSala(Sala *salaOrigem, string salaDestino);
	Mapa mapa;
	vector<Personagem*> getPessoasNaSala(Sala* sala);

	class No;

	void objetoOrdem(Objeto* objeto);
	void personagemOrdem(Personagem* personagem);
	Personagem* findCharacter(string nome);

	void update(int id) override;
	void advanceTime();

	void initializeGame();
	bool loadGame();
	void saveGame();

	void obterObjeto(string nome, Personagem* recebedor);

public:
	Jogo();

	void setup();
	Sala* getSalaAtual() { return jogador.getSalaAtual(); }
	void receberArgs(vector<string> args);

	vector<Item> getInventario();

	void imprimirTexto(string texto);
	string getTexto() { return texto; }

	enum { obter, imprimir };
};
#pragma once
#include "characters/Jenna.h"
#include "characters/Baxter.h"
#include "characters/Santos.h"
#include "characters/Hilda.h"
#include "jogador.h"
#include "IObservable.h"
#include "IObserver.h"
#include "mapa.h"
#include "conversa.h"
#include "fileManager.h"
#include "../libs/pugixml/src/pugixml.hpp"
#include "saveModule.h"

using namespace std;
using namespace pugi;

auto compare = [](NPC* a, NPC* b) -> bool { return a->getAction() < b->getAction(); };

class Jogo : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 500;

	int time;
	int loop;

	string texto;

	Jogador jogador;
	vector<Personagem*> personagens;
	vector<NPC*> npcs;
	vector<Conversa> conversas;

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
	void advanceConversas();

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
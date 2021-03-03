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

static auto actionCompare = [](NPC* a, NPC* b) -> bool { return a->getAction() < b->getAction(); };

class Jogo : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 500;

	int time;
	int loop;

	string texto;

	Jogador player;
	vector<Personagem*> characters;
	vector<NPC*> npcs;
	vector<Conversa> conversations;

	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;

	Sala* moveRoom(Sala *salaOrigem, string salaDestino);
	Mapa mapa;
	vector<Personagem*> getPeopleInRoom(Sala* room);

	class No;

	void objectAction(Object* object);
	void characterAction(Personagem* personagem);
	Personagem* findCharacter(string nome);

	void update(int id) override;
	void advanceTime();
	void advanceConversations();

	void initializeGame();
	bool loadGame();
	void saveGame();

	void obtainObject(string nome, Personagem* recebedor);

public:
	Jogo();

	void setup();
	Sala* getSalaAtual() { return player.getSalaAtual(); }
	void receiveArgs(vector<string> args);

	vector<Item> getInventory();

	void printText(string texto);
	string getText() { return texto; }

	enum {_obter, _imprimir};
};
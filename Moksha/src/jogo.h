#pragma once
#include "characters/Jenna.h"
#include "characters/Baxter.h"
#include "characters/Santos.h"
#include "characters/Hilda.h"
#include "jogador.h"
#include "IObservable.h"
#include "IObserver.h"
#include "mapa.h"
#include "conversation.h"
#include "fileManager.h"
#include "../libs/pugixml/src/pugixml.hpp"
#include "saveModule.h"

using namespace std;
using namespace pugi;

static auto actionCompare = [](Personagem* a, Personagem* b) -> bool { return a->getAction() < b->getAction(); };

class Jogo : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 500;

	int time;
	int loop;

	string text;

	Jogador* player;
	vector<Personagem*> characters;
	vector<NPC*> npcs;
	vector<Conversation> conversations;

	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;

	Sala* moveRoom(Sala *salaOrigem, string salaDestino);
	Mapa mapa;
	vector<Personagem*> getPeopleInRoom(Sala* room);

	class No;

	void objectAction(Object* object);
	void characterAction(Personagem* character);
	Personagem* findCharacter(string nome);
	void broadcastEvent(Personagem* emitter, vector<string> args);

	void update(int id) override;
	void advanceTime();
	void advanceConversations();

	void initializeGame();
	bool loadGame();
	void saveGame();

	void obtainObject(string name, Personagem* receiver);

public:
	Jogo();

	void setup();
	void receiveArgs(vector<string> args);

	vector<Item> getItems();
	vector<Concept> getConcepts();

	void printText(string text);
	string getText() { return text; }

	enum {_obter, _imprimir, _ouvir};
};
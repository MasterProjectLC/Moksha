#pragma once
#include "characters/Baxter.h"
#include "characters/Willow.h"
#include "characters/Magnus.h"
#include "characters/Santos.h"
#include "characters/Jenna.h"
#include "characters/Renard.h"
#include "characters/Paul.h"
#include "characters/Hilda.h"
#include "characters/Tom.h"
#include "characters/Liz.h"
#include "characters/George.h"
#include "characters/Amelie.h"
#include "player.h"
#include "IObservable.h"
#include "IObserver.h"
#include "map.h"
#include "conversation.h"
#include "fileManager.h"
#include "../libs/pugixml/src/pugixml.hpp"
#include "saveModule.h"

using namespace std;
using namespace pugi;

static auto actionCompare = [](Character* a, Character* b) -> bool { return a->getAction() < b->getAction(); };

class Game : public IObservable, public IObserver {
private:
	const int OBSERVER_OFFSET = 2000;

	int time;
	int loop;

	string text;

	Player* player;
	vector<Character*> characters;
	vector<NPC*> npcs;
	vector<Conversation> conversations;

	Room* moveRoom(Room *origin, string destination);
	Map map;
	vector<Character*> getPeopleInRoom(Room* room);

	class No;

	void objectAction(Object* object);
	void characterAction(Character* character);
	Character* findCharacter(string nome);
	void broadcastEvent(Character* emitter, vector<string> args);

	void update(int id) override;
	void advanceTime();
	void advanceConversations();

	void initializeGame();
	bool loadGame();
	void saveGame();

	void obtainObject(string name, Character* receiver);

public:
	Game();

	void setup();
	void receiveArgs(vector<string> args);

	vector<Item> getItems();
	vector<Concept> getConcepts();

	void printText(string text);
	string getText() { return text; }

	enum {_obter, _imprimir, _ouvir};
};
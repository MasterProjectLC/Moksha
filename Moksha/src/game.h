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
#include "interface.h"
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

static auto actionCompare = [](Character* a, Character* b) -> bool {
	if  (a->getAction() != b->getAction())
		return a->getAction() < b->getAction();
	else
		return (a->getName() == "Elliot");
};

class Game : public IObserver {
private:
	const int OBSERVER_OFFSET = 2000;

	int time;
	int loop;

	string text;

	Player* player;
	vector<Character*> characters;
	vector<NPC*> npcs;
	vector<Conversation*> conversations;

	Map map;
	vector<Character*> getPeopleInRoom(Room* room);

	class No;

	void characterAction(Character* character);
	void broadcastMessage(string topic, string str, Character* sender, vector<Character*> receivers, Room* room);
	void broadcastEvent(Character* emitter, vector<string> args);

	void update(int id) override;
	void advanceTime();
	void advanceConversations();

	void initializeGame();
	bool loadGame(string loadFile);
	void saveGame(string baseSave);
	void rewindGame();

	bool emitEvent(int id, vector<string> args);

	void updateNeighbours(Character* character);
	void addConversation(string convo, Character* starter, bool isReaction);
	void addListener(Character* character, string target);

public:
	Game();

	void setup();
	void receiveArgs(vector<string> args);

	string getText() { return text; }
	Character* findCharacter(string nome);

	enum {fim_conversa, passagem_tempo, inicio_conversa, item_deixado};
};
#pragma once
#include <queue>
#include "goal.h"
#include "priorityVector.h"
#include "character.h"
#include "map.h"
#include "fileManager.h"
#include "dictionary.h"
#include "../libs/GPGOAP/goap.h"
#include "../libs/GPGOAP/astar.h"  // ASTER GASTER WOWOWOWOW

using namespace std;

class NPC : public Character {
protected:
	string* description;

	set<string*> addedConditions;
	set<string*> addedActions;
	set<string> trackablePeople;

	queue<Room*> path;
	Map* map;
	string conversaAlvo;

	actionplanner_t ap;
	worldstate_t states[16];
	worldstate_t world;
	Goal currentGoal;
	PriorityVector<Goal> goalList;
	const char* plan[16]; // The planner will return the action plan in this array.
	int plansz = 16; // Size of our return buffers
	int planCost = 0;
	int currentStep = 0;
	Dictionary<vector<string>> dict;

	Dictionary<string> lastSeen;

	queue<Room*> findPath(Room* salaInicial, Room* salaAlvo);
	queue<Room*> findPath(Room* salaAlvo);
	queue<Room*> search();
	queue<Room*> search(Room* salaPista);

	int tamanhoCaminho(Room* inicio, Room* alvo) {
		return map->optimalPath(inicio, alvo).size();
	};

	string nextRoomInPath();

	virtual int decideActionParticular(string acao) { return descansar; }
	virtual void setupWorldParticular() {}
	virtual void setupObjectivesParticular() {}
	virtual void setupActionsParticular() {}
	void updateWorld();
	void updateLastSeen(string pursueTarget, string room);
	virtual void updateWorldExtra() {}
	void advancePlans();
	virtual void advancePlansExtra(string currentProcess) {}
	void changePlans() { changePlans(false); };
	void changePlans(bool justUpdated);

	bool isCurrentStateFulfilled();

public:
	explicit NPC(Map* m, string name, string description, int gender, int strength, int dexterity);

	string* getDescription() { return description; }

	void setCondition(string condition, bool update);
	bool hasCondition(string info) override;
	void takeAction() override { Character::takeAction(); updateWorld(); }
	void receiveEvent(vector<string> args) override;

	int decideAction();
	void executeReaction(string topic, string phrase, string sender, bool shouldRespond) override;
	void checkRoom(vector<Character*> peopleInRoom) override;
	void seeCharMoving(Character* character, string otherRoom, bool entering) override;
	void setSalaAlvo(Room* nova) { findPath(nova); }

	void setupPlans();

	vector<string> getActionList();
	vector<string> getAtomList();
};
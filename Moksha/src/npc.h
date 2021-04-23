#pragma once
#include <queue>
#include "goal.h"
#include "priorityVector.h"
#include "character.h"
#include "fileManager.h"
#include "dictionary.h"
#include "../libs/GPGOAP/goap.h"
#include "../libs/GPGOAP/astar.h"  // ASTER GASTER WOWOWOWOW
#include <functional>

using namespace std;

class NPC : public Character {
protected:
	string* description;

	set<string> topicList;
	set<string*> addedConditions;
	set<string*> addedActions;
	set<string> trackablePeople;
	set<string*> trackableRooms;
	set<string*> trackableConvos;
	map<string, tuple<string, string>> trackableNulls;
	vector<string*> conditionNames;

	queue<Room*> path;
	string conversaAlvo;

	actionplanner_t ap;
	worldstate_t stateZero;
	worldstate_t states[16];
	worldstate_t world;
	Goal currentGoal;
	PriorityVector<Goal> goalList;
	const char* plan[16]; // The planner will return the action plan in this array.
	int plansz = 16; // Size of our return buffers
	int planCost = 0;
	int currentStep = 0;

	map<string, vector<Room*>> lastSeen;

	queue<Room*> findPath(Room* initialRoom, Room* targetRoom);
	queue<Room*> findPath(Room* targetRoom);
	queue<Room*> search();
	queue<Room*> search(vector<Room*> breadcrumbs);

	int pathSize(Room* initial, Room* target) {
		return mapp->optimalPath(initial, target).size();
	};

	string* addTrackable(string trackable, set<string*>* trackableSet, string action_name, string condition_name);
	string* addTrackable(string trackable, set<string>* trackableSet, string action_name, string condition_name);
	string* addTrackableHelper(string action_name, string condition_name);
	void addTrackableRoom(string room);
	void addTrackablePeople(string person);
	void addTrackablePeople(string person, vector<string> breadcrumbs);
	void addTrackableConvo(string convo);
	void addTrackableConvo(string convo, string precondition);
	void addTrackableNull(string action, string condition, string room) { addTrackableNull(action, condition, condition, room); }
	void addTrackableNull(string action, string condition, string description, string room);

	void advancePath();
	void move(string room) override;
	void talk(string convo, bool isReaction) override;

	virtual void setupWorldParticular() {}
	virtual void setupObjectivesParticular() {}
	virtual void setupActionsParticular() {}

	void updateWorld();
	void updateWorldVariables();
	virtual void updateWorldExtra() {}
	void updateLastSeen(string pursueTarget, string room);

	void changePlans() { changePlans(false); };
	void changePlans(bool justUpdated);
	void advancePlans();
	void setupProcess(string currentProcess);
	virtual void setupProcessParticular(string currentProcess) {}

	virtual int decideActionParticular(string acao) { return descansar; }

	bool isCurrentStateFulfilled();
	void setBusy(bool novo) { busy = novo; }

	void setupCrewArea();

public:
	explicit NPC(Map* m, string name);
	~NPC();

	string* getDescription() { return description; }

	void setCondition(string condition, bool update);
	bool hasCondition(string info) override;
	void takeAction() override { Character::takeAction(); updateWorld(); }
	void receiveEvent(vector<string> args) override;

	int decideAction();
	void executeReaction(string topic, string phrase, string sender, bool shouldRespond) override;
	void checkRoomParticular(vector<Character*> peopleInRoom) override;
	void seeCharMovingParticular(Character* character, Room* otherRoom, bool entering) override;
	void setTargetRoom(Room* newRoom) { findPath(newRoom); }

	void createPlan();

	void clearGoals() { goalList.clear(); }
	void addGoal(vector<string*> conditions, vector<bool> conditionStates, int priority);
	void addGoal(string* condition, bool conditionState, int priority);
	void addGoal(bfield_t values, bfield_t dontcare, bool conditionState, int priority);

	void setupWorld();
	void clear();

	vector<string> getActionList();
	vector<string> getAtomList();
	vector<Goal> getGoalList() { return goalList.getVector(); }
};
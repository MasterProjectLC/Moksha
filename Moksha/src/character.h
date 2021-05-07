#pragma once
#include "map.h"
#include "inventory.h"
#include "dictionary.h"
#include "IObservable.h"
#include "actionEnum.h"

using namespace std;

class Character: public IObservable {
protected:
	const set<string> names = { "Elliot", "Baxter", "Willow", "Hilda", "Santos",
								"Magnus", "Tom", "Jenna", "Renard", "Liz",
								"George", "Paul", "Amelie" };

	string name;
	vector<string> notifyArgs;
	vector<Character*> notifyTargets;
	set<Room*> checkedRooms;
	string status;

	char gender;
	int strength;
	int dexterity;
	Dictionary<bool> statusEffects;
	bool unconscious;
	bool dead;
	bool busy;
	bool conversation;
	
	Map* mapp;
	Room *currentRoom;
	Inventory inventory;

	set<int> basicActions;
	int currentAction;
	vector<string> actionArgs;
	vector<Character*> neighbours;

	Character* findNeighbour(string name);
	void takeAction(int action, vector<string> args);
	void setBusy(bool novo) { busy = novo; }

public:
	Character() {}
	Character(char gender, int strength, int dexterity) {
		this->gender = gender;
		this->strength = strength;
		this->dexterity = dexterity;
		unconscious = false;
		dead = false;
		conversation = false;
		status = "doing nothing.";

		basicActions = { mover, descansar, mencionar, falar, conversar, interagir, deixar };
	}

	string getName() { return name; }
	int getAction() { return currentAction; }

	vector<Item*> getItems() { return inventory.getItems(); }
	vector<Concept*> getRumors() { return inventory.getRumors(); }
	vector<Concept*> getConcepts() { return inventory.getConcepts(); }
	virtual void addAbstract(string name, string codename, string description, char type) { inventory.addAbstract(name, codename, description, type); }
	virtual void addItem(string name, string codename, string description, set<string> actions) { inventory.addItem(name, codename, description, actions); }
	virtual void removeItem(string name) { inventory.removeItem(name); }

	void move(Room* room);
	virtual void move(string str);
	virtual void mention(string obj, set<string> receivers);
	virtual void mention(string obj, string receiver);
	void attack(string target);
	void leave(string objectName);
	void transform(string oldObjectName, string newObjectCodename);
	void listen(string target);
	void check(string target);
	void reveal(string info);
	void scan();
	virtual void say(string topic, string str, vector<Character*> receivers);
	void rest();
	void talk(string convo);
	virtual void talk(string convo, bool isReaction);
	void take(string objectName);
	void voidAction(string actionStatus);
	virtual void interact(string action, string object);

	string getStatus() { return status; }
	void setStatus(string n) { status = n; }
	void setCurrentRoom(Room *room) { currentRoom = room; }
	Room* getCurrentRoom() { return currentRoom; }
	char getGender() { return gender; }
	int getStrength() { return strength; }
	int getDexterity() { return dexterity; }
	bool isUnconscious() { return unconscious || dead; }
	bool isDead() { return dead; }
	bool isBusy() { return busy; }
	bool getStatusEffect(string key);
	bool inConversation() {	return conversation; }
	void setInConversation(bool a) { conversation = a; if (a) { setStatus("talking."); }; }

	bool isActionValid(int action);

	virtual void takeAction() { takeAction(currentAction, actionArgs); }
	void sayLine(string topic, string str, vector<Character*> receivers) { say(topic, str, receivers); }

	string getNotifyText() { return notifyArgs[0]; }
	vector<string> getNotifyArgs() { return notifyArgs; }
	vector<Character*> getNotifyTargets() { return notifyTargets; }

	virtual bool hasCondition(string info) { return false; }
	bool hasItem(string item) { return inventory.getItem(item) != NULL; }
	Item* getItem(string item) { return inventory.getItem(item); }

	void broadcastEvent(vector<string> args);
	virtual void receiveEvent(vector<string> args) {}
	virtual void receiveCheck(Character* checkTarget) {}
	virtual void executeReaction(string topic, string phrase, Character* sender, bool shouldRespond) {}
	void checkRoom(vector<Character*> peopleInRoom);
	virtual void checkRoomParticular(vector<Character*> peopleInRoom) {}
	void seeCharMoving(Character* person, Room* otherRoom, bool entering);
	virtual void seeCharMovingParticular(Character* person, Room* otherRoom, bool entering) {}
	virtual bool beAttacked(Character* attacker);

	void setNeighbours(vector<Character*> neighbours);
	void obtainAbstract(string codename);
	void obtainObject(string codename);
};

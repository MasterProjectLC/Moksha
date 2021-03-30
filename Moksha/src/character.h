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
	string notifyText;
	set<string> notifyTargets;
	set<Room*> checkedRooms;
	string status;

	int gender;
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

	void takeAction(int action, vector<string> args);
	virtual void move(string str);
	void move(Room* room);
	virtual void mention(string obj, set<string> receivers);
	virtual void mention(string obj, string receiver);
	void attack(string target);
	void leave(string target);
	void listen(string target);
	void check(string target);
	void scan();
	void say(string topico, string str, set<string> receivers);
	void rest();
	void talk(string convo);
	void voidAction(string actionStatus);
	virtual void interact(string action, string object);

	void setBusy(bool novo) { busy = novo; }

public:
	Character() {}
	Character(int gender, int strength, int dexterity) {
		this->gender = gender;
		this->strength = strength;
		this->dexterity = dexterity;
		unconscious = false;
		dead = false;
		conversation = false;
		status = "doing nothing.";

		basicActions = { mover, descansar, mencionar, falar, conversar, interagir };
	}

	string getName() { return name; }
	int getAction() { return currentAction; }

	vector<Item*> getItems() { return inventory.getItems(); }
	vector<Concept*> getRumors() { return inventory.getRumors(); }
	vector<Concept*> getConcepts() { return inventory.getConcepts(); }
	void addConcept(string name) { inventory.addConcept(name); }
	void addRumor(string name) { inventory.addRumor(name); }
	void addItem(string name, string codename, string description, set<string> actions) { inventory.addItem(name, codename, description, actions); }
	void removeItem(string name) { inventory.removeItem(name); }

	string getStatus() { return status; }
	void setStatus(string n) { status = n; }
	void setCurrentRoom(Room *room) { currentRoom = room; }
	Room* getCurrentRoom() { return currentRoom; }
	int getGender() { return gender; }
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
	void sayLine(string topic, string str, set<string> receivers) { say(topic, str, receivers); }
	void printText(string str);

	string getNotifyText() { return notifyText; }
	set<string> getNotifyTargets() { return notifyTargets; }

	virtual bool hasCondition(string info) { return false; }
	bool hasItem(string item) { return inventory.getItem(item) != NULL; }
	Item* getItem(string item) { return inventory.getItem(item); }

	virtual void receiveEvent(vector<string> args) {}
	virtual void receiveCheck(Character* checkTarget) {}
	virtual void executeReaction(string topic, string phrase, string sender, bool shouldRespond) {}
	virtual void checkRoom(vector<Character*> peopleInRoom) {}
	virtual void seeCharMoving(Character* pessoa, Room* otherRoom, bool entrando) {}
	virtual bool beAttacked(Character* atacante) {
		if (atacante->getStrength() >= strength)
			unconscious = true;
		else
			atacante->beAttacked(this);

		return unconscious;
	};

};

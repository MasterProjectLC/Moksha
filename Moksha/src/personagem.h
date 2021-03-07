#pragma once
#include "sala.h"
#include "inventory.h"
#include "dictionary.h"
#include "IObservable.h"
#include "actionEnum.h"

using namespace std;

class Personagem: public IObservable {
protected:
	string name;
	string notifyText;
	set<string> notifyTargets;
	set<Sala*> salasChecadas;
	string status;

	int gender;
	int strength;
	int dexterity;
	bool unconscious;
	bool dead;
	bool conversation;
	
	Sala *currentRoom;
	Inventory inventory;

	set<int> basicActions;
	int currentAction;
	vector<string> actionArgs;

	void takeAction(int action, vector<string> args);
	virtual void move(string str);
	void move(Sala sala);
	virtual void mention(string obj, set<string> receivers);
	virtual void mention(string obj, string receiver);
	void attack(string target);
	void listen(string target);
	void say(string topico, string str, set<string> receivers);
	void rest();
	void talk(string convo);
	virtual void interact(string action, string object);

public:
	Personagem() {}
	Personagem(int gender, int strength, int dexterity) {
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

	vector<Item> getInventory() { return inventory.getItems(); }
	void addConceito(string nome) { inventory.addConcept(nome); }
	void addItem(string nome, set<string> acoes) { inventory.addItem(nome, acoes); }

	string getStatus() { return status; }
	void setStatus(string n) { status = n; }
	void setCurrentRoom(Sala *room) { currentRoom = room; }
	Sala* getCurrentRoom() { return currentRoom; }
	int getGender() { return gender; }
	int getStrength() { return strength; }
	int getDexterity() { return dexterity; }
	bool isUnconscious() { return unconscious || dead; }
	bool isDead() { return dead; }
	bool inConversation() {	return conversation; }
	void setInConversation(bool a) { conversation = a; if (a) { setStatus("talking."); }; }

	bool isActionValid(int action);

	virtual void takeAction() { takeAction(currentAction, actionArgs); }
	void sayLine(string topico, string str, set<string> receivers) { say(topico, str, receivers); }
	void printText(string str);

	string getNotifyText() { return notifyText; }
	set<string> getNotifyTargets() { return notifyTargets; }

	virtual bool hasCondition(string info) { return false; }

	virtual void executeReaction(string topico, string frase, string remetente, bool shouldRespond) {}
	virtual void checkRoom(vector<Personagem*> pessoasNaSala) {}
	virtual void seeCharMoving(Personagem* pessoa, string outraSala, bool entrando) {}
	virtual void beAttacked(Personagem* atacante) {
		if (atacante->getStrength() >= strength)
			unconscious = true;
	};

};

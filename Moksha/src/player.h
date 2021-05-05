#pragma once
#include <map>
#include <set>
#include "npc.h"
#include "stringLib.h"
#include "fileManager.h"
#include "interface.h"

using namespace std;

class Player : public Character, public IObserver {
private:
	string noObjectError;
	string noPersonError;
	string noActionError;
	string noRoomError;
	string noItemError;
	string mindError;

	map<string, set<string>> mindTheory;
	Interface interfacer = Interface(140, 40, 60, 30);

	void interact(string action, string object) override;
	void say(string topic, string str, vector<Character*> receivers) override;
	bool characterCheck(vector<string> args);

	void addToMind(string topic, string character);

public:
	Player() {}
	Player(Map* mapp);
	void bootGame();
	void update(int id) override;
	void receiveArgs(vector<string> args);

	bool hasCondition(string info) override;

	void addAbstract(string name, string codename, string description, char type) override;
	void addItem(string name, string codename, string description, set<string> actions) override;
	void removeItem(string name) override;

	void seeCharMovingParticular(Character* person, Room* otherRoom, bool entering) override;
	void executeReaction(string topic, string phrase, Character* sender, bool shouldRespond) override;
	void receiveCheck(Character* checkTarget) override;
	void checkRoomParticular(vector<Character*> peopleInRoom) override;
	void updateRoom(vector<Character*> peopleInRoom);
	void printNeighbours();

	bool beAttacked(Character* attacker) override {
		printText(attacker->getName() + " attacked you!");
		return Character::beAttacked(attacker);
	};

	void rewind();
	void printText(string str) { printText(str, 'w'); }
	void printMaleText(string str) { printText(str, 'c'); }
	void printFemaleText(string str) { printText(str, 'r'); }
	void printGameText(string str) { printText(str, 'w'); }
	void printMindText(string str) { printText(str, 'y'); }
	void printNarratorText(string str) { printText(str, 'p'); }
	void printText(string str, char color) { interfacer.printLine(str, color); }
};
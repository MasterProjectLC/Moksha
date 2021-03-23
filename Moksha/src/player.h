#pragma once
#include <map>
#include <set>
#include "npc.h"
#include "stringLib.h"
#include "fileManager.h"

using namespace std;

class Player : public Character {
private:
	string noObjectError;
	string noPersonError;
	string noActionError;
	string noRoomError;
	string noItemError;
	string mindError;

	map<string, set<string>> mindTheory;

	void interact(string action, string objeto) override;

	bool characterCheck(vector<string> args);

	void addToMind(string topic, string character);

public:
	Player() {}
	Player(Map* mapp);
	void receiveArgs(vector<string> args);

	bool hasCondition(string info) override;

	void seeCharMoving(Character* person, Room* otherRoom, bool entering) override;
	void executeReaction(string topic, string phrase, string sender, bool shouldRespond) override;
	void receiveCheck(Character* checkTarget) override;
	void checkRoom(vector<Character*> peopleInRoom) override;
	void updateRoom(vector<Character*> peopleInRoom);

	bool beAttacked(Character* attacker) override {
		printText(attacker->getName() + " attacked you!");
		return Character::beAttacked(attacker);
	};
};
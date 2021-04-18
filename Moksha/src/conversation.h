#pragma once
#include <vector>
#include <set>
#include <string>
#include "character.h"
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

typedef struct {
	string infoAtom;
	string line;
	string speaker;
} message;

class Conversation {
private:
	int convoStage;
	string name;
	xml_document conversation;
	xml_node_iterator it;
	vector<Character*> participants;
	vector<Character*> listeners;
	set<string> tags;
	string room;
	bool begun;
	bool isReaction;

	Character* findParticipant(string name);

public:
	Conversation() {}
	Conversation(string conversation, string room, vector<Character*> characters);
	Conversation(string conversation, string room, vector<Character*> characters, bool reaction);
	Conversation(string conversation, string room, vector<Character*> characters, bool reaction, int stage);

	bool participates(Character* name); // If given character participates in the convo
	bool hasTag(string tag);

	xml_node nextLine();
	bool ended();

	void addTag(string tag) { tags.insert(tag); }
	void addListener(Character* listener) { participants.push_back(listener); listeners.push_back(listener); }
	void clearListeners();

	bool advance(message* returned);
	vector<Character*> getParticipants() { return participants; }
	vector<Character*> getParticipants(Character* removed);
	string getName() { return name; }
	string getRoom() { return room; }
	int getStage() { return convoStage; }
	bool getIsReaction() { return isReaction; }
};

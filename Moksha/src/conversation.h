#pragma once
#include <vector>
#include <set>
#include <string>
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

class Conversation {
private:
	int convoStage;
	string name;
	xml_document conversation;
	xml_node_iterator it;
	set<string> participants;
	set<string> listeners;
	set<string> tags;
	string room;
	bool begun;
	bool isReaction;

public:
	Conversation() {}
	Conversation(string conversation, string room);
	Conversation(string conversation, string room, bool reaction);
	Conversation(string conversation, string room, bool reaction, int stage);

	bool participates(string name); // If given character participates in the convo
	bool hasTag(string tag);

	xml_node nextLine();
	bool ended();

	void addTag(string tag) { tags.insert(tag); }
	void addListener(string listener) { participants.insert(listener); listeners.insert(listener); }
	void clearListeners();

	set<string>* getParticipants() { return &participants; }
	set<string> getParticipants(string removed);
	string getName() { return name; }
	string getRoom() { return room; }
	int getStage() { return convoStage; }
	bool getIsReaction() { return isReaction; }
};

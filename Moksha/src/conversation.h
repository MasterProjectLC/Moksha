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
	string room;
	bool begun;

public:
	Conversation() {}
	Conversation(string conversation, string room);
	Conversation(string conversation, string room, int stage);

	bool participates(string nome); // Se personagem citado participa da conversa ou não

	xml_node nextLine();
	bool ended();

	set<string>* getParticipants() { return &participants; }
	set<string> getParticipants(string removido);
	void addListener(string listener) { participants.insert(listener); listeners.insert(listener); }
	void clearListeners();

	string getName() { return name; }
	string getRoom() { return room; }
	int getStage() { return convoStage; }
};

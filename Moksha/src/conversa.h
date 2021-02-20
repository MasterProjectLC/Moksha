#pragma once
#include <vector>
#include <set>
#include <string>
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

class Conversa {
private:
	int convoStage;
	xml_document conversation;
	xml_node_iterator it;
	set<string> participants;
	string room;

public:
	Conversa() {}
	Conversa(string conversa, string sala);
	Conversa(string conversa, string sala, int stage);

	bool participates(string nome); // Se personagem citado participa da conversa ou não

	xml_node nextLine();
	bool ended();

	set<string>* getParticipants() { return &participants; }
	set<string> getParticipants(string removido);

	string getRoom() { return room; }
	int getStage() { return convoStage; }
};

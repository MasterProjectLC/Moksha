#include "conversation.h"

Conversation::Conversation(string conversation, string room) {
	name = conversation;
	begun = true;
	convoStage = 0;
	this->conversation.load_file( ("files/conversations/" + conversation + ".xml").c_str() );
	it = this->conversation.child("Conversation").begin();
	this->room = room;

	// Inserir participantes
	for (xml_node_iterator ait = this->conversation.child("Participants").begin(); ait != this->conversation.child("Participants").end(); ait++) {
		this->participants.insert(ait->name());
	}
}

Conversation::Conversation(string conversation, string room, int stage) : Conversation(conversation, room) {
	for (int i = 0; i < stage; i++) {
		nextLine();
	}
}

bool Conversation::participates(string name) {
	return participants.count(name) > 0;
}

bool Conversation::hasTag(string tag) {
	return tags.count(tag) > 0;
}

xml_node Conversation::nextLine() {
	convoStage++;
	return *(it++);
}

bool Conversation::ended() {
	return it == conversation.child("Conversation").end();
}


void Conversation::clearListeners() {
	for (set<string>::iterator it = listeners.begin(); it != listeners.end(); ++it)
		participants.erase(*it);

	listeners.clear();
}


set<string> Conversation::getParticipants(string removed) {
	set<string> retorno = *getParticipants();
	
	for (set<string>::iterator it = retorno.begin(); it != retorno.end(); it++) {
		if ((*it) == removed) {
			retorno.erase(it);
			break;
		}
	}

	return retorno;
}
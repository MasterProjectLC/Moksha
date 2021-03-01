#include "conversa.h"

Conversa::Conversa(string conversation, string room) {
	name = conversation;
	convoStage = 0;
	this->conversation.load_file( ("files/conversations/" + conversation + ".xml").c_str() );
	it = this->conversation.child("Conversation").begin();
	this->room = room;

	// Inserir participantes
	for (xml_node_iterator ait = this->conversation.child("Participants").begin(); ait != this->conversation.child("Participants").end(); ait++) {
		this->participants.insert(ait->name());
	}
}

Conversa::Conversa(string conversation, string room, int stage) : Conversa(conversation, room) {
	for (int i = 0; i < stage; i++) {
		nextLine();
	}
}

bool Conversa::participates(string nome) {
	return participants.count(nome) > 0;
}

xml_node Conversa::nextLine() {
	convoStage++;
	return *(it++);
}

bool Conversa::ended() {
	return it == conversation.child("Conversation").end();
}


set<string> Conversa::getParticipants(string removed) {
	set<string> retorno = *getParticipants();
	
	for (set<string>::iterator it = retorno.begin(); it != retorno.end(); it++) {
		if ((*it) == removed) {
			retorno.erase(it);
			break;
		}
	}

	return retorno;
}
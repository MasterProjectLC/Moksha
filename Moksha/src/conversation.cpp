#include "conversation.h"

Conversation::Conversation(string conversation, string room, vector<Character*> participants) : Conversation(conversation, room, participants, false) {}

Conversation::Conversation(string conversation, string room, vector<Character*> participants, bool isReaction) {
	name = conversation;
	begun = true;
	convoStage = 0;
	this->isReaction = isReaction;
	if (isReaction)
		this->conversation.load_file(("files/reactions/" + conversation + ".xml").c_str());
	else
		this->conversation.load_file(("files/conversations/" + conversation + ".xml").c_str());
	it = this->conversation.child("Conversation").begin();
	this->room = room;

	// Insert participants
	for (xml_node_iterator ait = this->conversation.child("Participants").begin(); ait != this->conversation.child("Participants").end(); ait++) {
		for (vector<Character*>::iterator nit = participants.begin(); nit != participants.end(); nit++)
			if (ait->name() == (*nit)->getName())
				this->participants.push_back(*nit);
	}
}

Conversation::Conversation(string conversation, string room, vector<Character*> participants, bool isReaction, int stage)
	: Conversation(conversation, room, participants, isReaction) {
	for (int i = 0; i < stage; i++) {
		nextLine();
	}
}


bool Conversation::advance(message* returned) {
	// Try until a message shoots through
	bool endConvo = false;
	while (1) {
		// Advance
		xml_node conversation = nextLine();

		Character* speaker = NULL;
		bool isNarrator = (string(conversation.name()) == "Narrator");
		if (!isNarrator) {
			speaker = findParticipant(conversation.name());
			// Test if valid
			// Is the speaker in the room?
			if (!speaker || speaker->getCurrentRoom()->getCodename() != getRoom())
				continue;
		}

		// Does the speaker fulfill the necessary conditions?
		string infoAtom = ""; bool valid = true;
		for (xml_node_iterator cit = conversation.begin(); cit != conversation.end(); ++cit) {
			// Adding tags/infos
			string addedTag = cit->attribute("add_tag").value();
			string info = cit->attribute("info").value();
			string end = cit->name();

			// Check modifiers
			string tag = cit->attribute("tag").value();
			bool checkTag = (tag == "tag");

			bool conditionMet = (!checkTag && (isNarrator || speaker->hasCondition(cit->name()))) || (checkTag && hasTag(cit->name()));
			string nao = cit->attribute("n").value();
			bool inverted = (nao == "n");

			if (addedTag == "add_tag")
				addTag(cit->name());
			else if (info == "info")
				infoAtom = cit->name();
			else if (end == "end")
				endConvo = true;
			else if (conditionMet == inverted) {
				valid = false;
				break;
			}
		}

		if (!valid)
			continue;

		// Send message
		returned->infoAtom = infoAtom;
		returned->line = conversation.attribute("line").value();
		returned->speaker = conversation.name();

		// Lock every participant
		vector<Character*> participants = getParticipants();
		for (vector<Character*>::iterator ait = participants.begin(); ait != participants.end(); ait++) {
			if ((*ait)->getCurrentRoom()->getCodename() == getRoom())
				(*ait)->setInConversation(true);
		}
		break;
	}

	// End convo?
	// If the convo is over
	if (endConvo || ended())
		return true;
	return false;
}


bool Conversation::participates(Character* chara) {
	for (vector<Character*>::iterator ait = participants.begin(); ait != participants.end(); ait++)
		if ((*ait) == chara)
			return true;
	return false;
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
	for (vector<Character*>::iterator it = listeners.begin(); it != listeners.end(); it++)
		for (vector<Character*>::iterator ait = participants.begin(); ait != participants.end(); ait++)
			if ((*it)->getName() == (*ait)->getName()) {
				participants.erase(ait);
				break;
			}

	listeners.clear();
}


vector<Character*> Conversation::getParticipants(Character* removed) {
	vector<Character*> retorno = getParticipants();
	
	for (vector<Character*>::iterator it = retorno.begin(); it != retorno.end(); it++) {
		if ((*it) == removed) {
			retorno.erase(it);
			break;
		}
	}

	return retorno;
}


Character* Conversation::findParticipant(string name) {
	for (vector<Character*>::iterator it = participants.begin(); it != participants.end(); it++)
		if ((*it)->getName() == name)
			return *it;
}
#include "game.h"
#include <iostream>

int main() {
	Game g = Game();
	return 0;
};

Game::Game() {
	time = 0;
	loop = 0;
	setup();
}


void Game::setup() {
	initializeGame();
	if (!loadGame("save.xml")) {
		// Literally first boot
		for (int i = 0; i < npcs.size(); i++) {
			npcs[i]->setupWorld();
		}

		conversations.push_back(new Conversation("intro", "FlightDock", characters, false));
		advanceConversations();
	}

	if (findCharacter("Baxter") == NULL) {
		((NPC*)findCharacter("Jenna"))->addGoal(new string("waiting_Runway"), true, 1000);
		((NPC*)findCharacter("George"))->addGoal(new string("waiting_Runway"), true, 1000);
	}

	player->bootGame();

}


// EVENTS
void Game::emitEvent(int id, vector<string> args) {
	switch (id) {
	case inicio_conversa:
		if (player->getCurrentRoom()->getCodename() == args[1]) {
			player->printText(args[0] + " has begun talking.");
		}
		break;

	case fim_conversa:
		if (args[0] == "journalist_rivalry_Jenna") {
			rewindGame();
		}
		break;

	case item_deixado:
		if (args[0] == "JennaSuitcase") {
			player->printText("Press Enter to continue.");
			conversations.push_back(new Conversation("intro2", "Runway", characters));
		}
		break;

	case passagem_tempo:
		if ((NPC*)findCharacter("Baxter") == NULL)
			return;

		switch (time) {
		case 35:
			((NPC*)findCharacter("Renard"))->addGoal(new string("taking_photos"), true, 50);
			((NPC*)findCharacter("Paul"))->addGoal(new string("the_medusa"), true, 50);
			break;

		case 40:
			((NPC*)findCharacter("Jenna"))->addGoal(new string("convo_renard_questionnaire"), true, 50);
			break;

		case 45:
			//((NPC*)findCharacter("Baxter"))->addGoal(new string("convo_presentation"), true, 50);
			((NPC*)findCharacter("Baxter"))->addGoal(new string("convo_familiar_faces"), true, 45);
			break;

		case 52:
			((NPC*)findCharacter("Jenna"))->addGoal(new string("medusa"), true, 50);
			((NPC*)findCharacter("Renard"))->addGoal(new string("medusa"), true, 50);
			((NPC*)findCharacter("Amelie"))->addGoal(new string("medusa"), true, 50);
			((NPC*)findCharacter("George"))->addGoal(new string("medusa"), true, 50);
			((NPC*)findCharacter("Magnus"))->addGoal(new string("medusa"), true, 50);
			break;

		case 70:
			((NPC*)findCharacter("Jenna"))->addGoal(new string("convo_jenna_amelie"), true, 40);
			break;

		case (60+60+30):
			((NPC*)findCharacter("George"))->addGoal(new string("convo_begin_card_game"), true, 50);
			((NPC*)findCharacter("George"))->addGoal(new string("convo_begin_card_game"), true, 1);
			((NPC*)findCharacter("Jenna"))->addGoal(new string("convo_begin_card_game"), true, 1);
			break;
		}
		break;
	}
}

// UPDATE --------------------------------------------------------
void Game::update(int id) {
	// Character
	if (id >= OBSERVER_OFFSET)
		characterAction(characters[id - OBSERVER_OFFSET]);
}


void Game::characterAction(Character* character) {
	int id = character->getNotifyID();
	Character* target;
	Room* oldRoom;
	Conversation* convo;

	switch (id) {
	case avancar:
		advanceTime();
		break;

	case tempo:
		player->printText(to_string(7 + (time / 60)) + ":" + to_string(time % 60));
		break;

	case mover:
		// Characters see char leaving
		for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); it++)
			if (*it != character && (*it)->getCurrentRoom() == character->getCurrentRoom())
				(*it)->seeCharMoving(character, map.getRoom(character->getNotifyText()), false);

		// Char enters the room
		oldRoom = character->getCurrentRoom();
		character->setCurrentRoom(moveRoom(character->getCurrentRoom(), character->getNotifyText()));
		if (character != player)
			character->checkRoom( getPeopleInRoom(character->getCurrentRoom()) );
		else
			player->updateRoom(getPeopleInRoom(player->getCurrentRoom()));

		// Characters see char entering
		for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); it++)
			if (*it != character && (*it)->getCurrentRoom() == character->getCurrentRoom())
				(*it)->seeCharMoving(character, oldRoom, true);
		break;

	case conversar:
		convo = new Conversation(character->getNotifyText(), character->getCurrentRoom()->getCodename(), characters,
			character->getNotifyArgs()[1][0] == 'r');
		conversations.push_back(convo);
		emitEvent(inicio_conversa, vector<string>({ character->getName(), convo->getRoom() }));
		// Preemptively lock everyone
		for (vector<Character*>::iterator it = convo->getParticipants().begin(); it != convo->getParticipants().end(); it++) {
			if ((*it)->getCurrentRoom()->getCodename() == convo->getRoom())
				(*it)->setInConversation(true);
		}
		break;

	case ouvir:
		target = findCharacter(character->getNotifyText());
		for (int i = 0; i < conversations.size(); i++) {
			if (conversations[i]->participates(target)) {
				conversations[i]->addListener(character);
					break;
			}
		}
		break;

	case deixar:
		Item* item = character->getItem(character->getNotifyText());
		string codename = item->getCodename();
		if (item != NULL && item->isActionValid("leave"))
			emitEvent(item_deixado, vector<string>({ codename }));
		break;
	}

}


void Game::advanceTime() {
	// Decide action
	for (int i = 0; i < npcs.size(); i++) {
		int action = npcs[i]->decideAction();
	}
	if (player->getAction() == ouvir)
		player->takeAction();

	// Conversations
	advanceConversations();

	// Order by priority
	PriorityVector<Character*> orderAction = PriorityVector<Character*>(vector<Character*>(), actionCompare);
	for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); it++) {
		if ((*it)->getAction() != ouvir)
			orderAction.push((*it));
	}

	// Take action
	while (!orderAction.empty()) {
		(*orderAction.highest())->takeAction();
		orderAction.pop();
	}
	player->setInConversation(false);

	// Update the player's room status
	for (int i = 0; i < npcs.size(); i++)
		npcs[i]->checkRoom( getPeopleInRoom(npcs[i]->getCurrentRoom()) );

	// Jogo
	time++;
	emitEvent(passagem_tempo, vector<string>());
	saveGame("save.xml");
}


void Game::advanceConversations() {
	if (conversations.empty())
		return;

	// Iterate through each current convo
	for (vector<Conversation*>::iterator it = conversations.begin(); it != conversations.end();) {
		message receiver = {"","",""};
		bool convoEnded = (*it)->advance(&receiver);
		if (receiver.speaker == "Narrator")
			broadcastMessage(receiver.infoAtom, receiver.line, "", (*it)->getParticipants(), map.getRoom((*it)->getRoom()));
		else
			findCharacter(receiver.speaker)->sayLine(receiver.infoAtom, receiver.line, (*it)->getParticipants());

		if (convoEnded) {
			string s = (*it)->getName();
			delete *it;
			it = conversations.erase(it);
			emitEvent(fim_conversa, vector<string>({ s }));
		}
		// Advance to next convo
		else
			it++;

		if (conversations.empty() || it == conversations.end())
			break;
	}
}


void Game::receiveArgs(vector<string> args) {
	player->receiveArgs(args);
}


void Game::broadcastMessage(string topic, string str, string sender, vector<Character*> receivers, Room* room) {
	for (vector<Character*>::iterator it = receivers.begin(); it != receivers.end(); it++) {
		if ((*it)->getCurrentRoom() == room) {
			(*it)->executeReaction(topic, str, sender, false);
			if ((*it) == player && topic != "") {
				player->obtainAbstract(topic);
			}
		}
	}
}

void Game::broadcastEvent(Character* emitter, vector<string> args) {
	vector<Character*> receivers = getPeopleInRoom(emitter->getCurrentRoom());
	for (int i = 0; i < receivers.size(); i++)
		if (receivers[i] != emitter)
			receivers[i]->receiveEvent(args);
}


// LIDAR COM MAPA ------------------------------------------

vector<Character*> Game::getPeopleInRoom(Room* room) {
	vector<Character*> retorno;
	for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); it++) {
		if ((*it)->getCurrentRoom() == room)
			retorno.push_back((*it));
	}

	return retorno;
}


// ACTIONS ----------------------------------
Room* Game::moveRoom(Room* origin, string destination) {
	// Movement
	origin = map.getRoom(destination);
	return origin;
}


// HELPER FUNCTIONS ---------------------------------------------
#include <stdexcept>
Character* Game::findCharacter(string name) {
	for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); it++) {
		if ((*it)->getName() == name)
			return (*it);
	}

	return NULL;
}
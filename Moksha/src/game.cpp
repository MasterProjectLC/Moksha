#include "game.h"
#include <iostream>

Game::Game() {
	Interface interfacer = Interface(140, 40, 60, 30);
	interfacer.add(this, 0);
	frameTime = 0;
	time = 0;
	loop = 0;
	setup();
}

int main() {
	Game g = Game();
	g.main();
};


void Game::setup() {
	initializeGame();
	if (!loadGame("save.xml")) {
		// Literally first boot
		for (int i = 0; i < npcs.size(); i++) {
			npcs[i]->setupWorld();
		}

		conversations.push_back(new Conversation("intro", "FlightDock", false));
		advanceConversations();
	}

	if (findCharacter("Baxter") == NULL) {
		((NPC*)findCharacter("Jenna"))->addGoal(new string("waiting_Runway"), true, 1000);
		((NPC*)findCharacter("George"))->addGoal(new string("waiting_Runway"), true, 1000);
	}

}

int Game::main() {
	while (1) {
		time = (int)timeGetTime();
		interfacer.clocking();
		time = (int)timeGetTime() - time;
		if (frameDelay > time)
			Sleep(frameDelay - time);
	}

	return 0;
}


// EVENTS
void Game::emitEvent(int id, vector<string> args) {
	switch (id) {
	case _evento_inicio_conversa:
		if (player->getCurrentRoom()->getCodename() == args[1]) {
			printText(args[0] + " has begun talking.");
		}
		break;

	case _evento_fim_conversa:
		if (args[0] == "journalist_rivalry_Jenna") {
			rewindGame();
		}
		break;

	case _evento_passagem_tempo:
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
	// Object
	if (id < OBSERVER_OFFSET) {
		objectAction(map.getObject(id));
	}

	// Character
	else {
		characterAction(characters[id - OBSERVER_OFFSET]);
	}
}


void Game::objectAction(Object* object) {
	// Obtain
	Character *character = findCharacter(object->getUser());

	switch (object->getNotifyID()) {
	case object->obter:
		obtainObject(object->getCodename(), character);					// Give object to character
		character->setStatus("obtaining " + object->getName() + ".");	// Update char's status
		character->getCurrentRoom()->removeObject(object);				// Remove object from the room
		break;

	case object->mover:
		character->move(object->getArgs()[0]);
		break;

	case object->tempo:
		printText(to_string(7+(time/60)) + ":" + to_string(time % 60));
		break;
	}
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

	case imprimir:
		printText(character->getNotifyText());
		break;

	case mover:
		// Characters see char leaving
		for (int i = 0; i < characters.size(); i++)
			if (characters[i] != character && characters[i]->getCurrentRoom() == character->getCurrentRoom())
				characters[i]->seeCharMoving(character, map.getRoom(character->getNotifyText()), false);

		// Char enters the room
		oldRoom = character->getCurrentRoom();
		character->setCurrentRoom(moveRoom(character->getCurrentRoom(), character->getNotifyText()));
		if (character != player)
			character->checkRoom( getPeopleInRoom(character->getCurrentRoom()) );
		else
			player->updateRoom(getPeopleInRoom(player->getCurrentRoom()));

		// Characters see char entering
		for (int i = 0; i < characters.size(); i++)
			if (characters[i] != character && characters[i]->getCurrentRoom() == character->getCurrentRoom())
				characters[i]->seeCharMoving(character, oldRoom, true);
		break;

	case mencionar:
		for (int i = 0; i < characters.size(); i++) {
			if (character->getNotifyTargets().count(characters[i]->getName()) && characters[i]->getCurrentRoom() == character->getCurrentRoom()) {
				if (characters[i]->isBusy())
					character->executeReaction("busy", "", characters[i]->getName(), false);
				else {
					characters[i]->executeReaction(character->getNotifyText(), "", character->getName(), true);
				}
				break;
			}
		}
		break;

	case falar:
		broadcastMessage(character->getNotifyArgs()[0], character->getNotifyArgs()[1], character->getName(), 
						character->getNotifyTargets(), character->getCurrentRoom());
		break;

	case conversar:
		convo = new Conversation(character->getNotifyText(), character->getCurrentRoom()->getCodename(),
			character->getNotifyArgs()[1][0] == 'r');
		conversations.push_back(convo);
		emitEvent(_evento_inicio_conversa, vector<string>({ character->getName(), convo->getRoom() }));
		// Preemptively lock everyone
		for (set<string>::iterator it = convo->getParticipants()->begin(); it != convo->getParticipants()->end(); it++) {
			Character* toLock = findCharacter(*it);
			if (toLock->getCurrentRoom()->getCodename() == convo->getRoom())
				toLock->setInConversation(true);
		}
		break;

	case ouvir:
		for (int i = 0; i < conversations.size(); i++)
			if (conversations[i]->participates(character->getNotifyText())) {
				conversations[i]->addListener(character->getName());
				break;
			}
		break;

	case checar:
		target = findCharacter(character->getNotifyText());
		if (target->getCurrentRoom() == character->getCurrentRoom())
			character->receiveCheck(target);
		break;


	case sondar:
		character->checkRoom( getPeopleInRoom(character->getCurrentRoom()) );
		break;

	case descansar:
		break;

	case atacar:
		target = findCharacter(character->getNotifyText());
		if (target->getCurrentRoom() == character->getCurrentRoom())
			if (target->beAttacked(character))
				broadcastEvent(character, vector<string>({ "attack", target->getName() }));
			else
				broadcastEvent(target, vector<string>({ "attack", character->getName() }));
		break;

	case deixar:
		Item* item = character->getItem(character->getNotifyText());
		string codename = item->getCodename();
		if (item != NULL && item->isActionValid("leave")) {
			FileDict fileDict = FileManager::readFromFile("files/objects/" + codename + ".txt");
			character->getCurrentRoom()->addObject(new Object(fileDict));
			character->removeItem(codename);
			notify(_obter);

			// TODO: maybe change this scripted thing later? It's not an often used action though, so it might not be worth it
			if (codename == "JennaSuitcase") {
				printText("Press Enter to continue.");
				conversations.push_back(new Conversation("intro2", "Runway"));
			}
		}
		
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
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getAction() != ouvir)
			orderAction.push(characters[i]);
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
	emitEvent(_evento_passagem_tempo, vector<string>());
	saveGame("save.xml");
}


void Game::advanceConversations() {
	if (conversations.empty())
		return;

	// Iterate through each current convo
	for (vector<Conversation*>::iterator it = conversations.begin(); it != conversations.end();) {
		// Try until a message shoots through
		bool endConvo = false;
		while (1) {
			// Advance
			xml_node conversation = (*it)->nextLine();

			Character* speaker = NULL;
			bool isNarrator = (string(conversation.name()) == "Narrator");
			if (!isNarrator) {
				speaker = findCharacter(conversation.name());
				// Test if valid
				// Is the speaker in the room?
				if (!speaker || speaker->getCurrentRoom()->getCodename() != (*it)->getRoom())
					continue;
			}

			// Does the speaker fulfill the necessary conditions?
			string infoAtom = ""; bool valid = true;
			for (xml_node_iterator cit = conversation.begin(); cit != conversation.end(); ++cit) {
				// Adding tags/infos
				string addTag = cit->attribute("add_tag").value();
				string info = cit->attribute("info").value();
				string end = cit->name();

				// Check modifiers
				string tag = cit->attribute("tag").value();
				bool checkTag = (tag == "tag");

				bool conditionMet = ( !checkTag && (isNarrator || speaker->hasCondition(cit->name())) ) || ( checkTag && (*it)->hasTag(cit->name()) );
				string nao = cit->attribute("n").value();
				bool inverted = (nao == "n");
				
				if (addTag == "add_tag")
					(*it)->addTag(cit->name());
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
			if (isNarrator)
				broadcastMessage( infoAtom, conversation.attribute("line").value(), "", *((*it)->getParticipants()), map.getRoom((*it)->getRoom()) );
			else
				speaker->sayLine( infoAtom, conversation.attribute("line").value(), *((*it)->getParticipants()) );

			// Lock every participant
			for (set<string>::iterator ait = (*it)->getParticipants()->begin(); ait != (*it)->getParticipants()->end(); ait++) {
				Character* toLock = findCharacter(*ait);
				if (toLock->getCurrentRoom()->getCodename() == (*it)->getRoom())
					toLock->setInConversation(true);
			}
			(*it)->clearListeners();
			break;
		}

		// End convo?
		// If the convo is over
		if (endConvo || (*it)->ended()) {
			string s = (*it)->getName();
			delete *it;
			it = conversations.erase(it);
			emitEvent(_evento_fim_conversa, vector<string>({ s }));
		}
		// Advance to next convo
		else {
			it++;
		}

		if (conversations.empty() || it == conversations.end())
			break;
	}
}


void Game::receiveArgs(vector<string> args) {
	player->receiveArgs(args);
}


void Game::broadcastMessage(string topic, string str, string sender, set<string> receivers, Room* room) {
	for (int i = 0; i < characters.size(); i++) {
		if (receivers.count(characters[i]->getName()) && characters[i]->getCurrentRoom() == room) {
			characters[i]->executeReaction(topic, str, sender, false);
			if (characters[i] == player && topic != "") {
				obtainAbstract(topic, player);
				notify(_ouvir);
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
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getCurrentRoom() == room)
			retorno.push_back(characters[i]);
	}

	return retorno;
}


// ACTIONS ----------------------------------
void Game::obtainObject(string codename, Character* receiver) {
	FileDict filedict = FileManager::readFromFile("files/items/" + codename + ".txt");
	vector<string> actionVec = filedict.getValues("actions");
	set<string> actionSet = set<string>();
	for (int i = 0; i < actionVec.size(); i++)
		actionSet.insert(actionVec[i]);

	if (filedict.hasKey("codename"))
		receiver->addItem(filedict.getValues("name")[0], filedict.getValues("codename")[0], "", actionSet);
	else
		receiver->addItem(filedict.getValues("name")[0], filedict.getValues("name")[0], "", actionSet);

	notify(_obter);
}


void Game::obtainAbstract(string codename, Character* receiver) {
	FileDict filedict = FileManager::readFromFile("files/abstract/" + codename + ".txt");

	if (filedict.hasKey("codename"))
		receiver->addAbstract(filedict.getValues("name")[0], filedict.getValues("codename")[0], filedict.getValues("description")[0], filedict.getValues("type")[0][0]);
	else
		receiver->addAbstract(filedict.getValues("name")[0], filedict.getValues("name")[0], filedict.getValues("description")[0], filedict.getValues("type")[0][0]);

	notify(_obter);
}


Room* Game::moveRoom(Room* origin, string destination) {
	// Movement
	origin = map.getRoom(destination);
	return origin;
}


void Game::printText(string text) {
	this->text = text;
	notify(_imprimir);
}


vector<Item*> Game::getItems() {
	return player->getItems();
}

vector<Concept*> Game::getConcepts() {
	return player->getConcepts();
}


// HELPER FUNCTIONS ---------------------------------------------
#include <stdexcept>
Character* Game::findCharacter(string name) {
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getName() == name)
			return characters[i];
	}

	return NULL;
}
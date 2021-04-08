#include "game.h"
#include <iostream>

Game::Game() {
	time = 0;
	loop = 0;
}


void Game::setup() {
	initializeGame();
	if (!loadGame("save.xml")) {
		// Literally first boot
		for (int i = 0; i < npcs.size(); i++) {
			npcs[i]->setupWorld();
		}

		conversations.push_back(Conversation("intro", "FlightDock", false));
		advanceConversations();
	}

	if (findCharacter("Baxter") == NULL) {
		((NPC*)findCharacter("Jenna"))->addGoal(new string("waiting_Runway"), true, 1000);
		((NPC*)findCharacter("George"))->addGoal(new string("waiting_Runway"), true, 1000);
	}

}


// EVENTS
void Game::emitEvent(int id, vector<string> args) {
	switch (id) {
	case _evento_fim_conversa:
		if (args[0] == "jenna_rivalry") {
			rewindGame();
		}
		break;
	}
}

// SAVE/LOAD --------------------------------------
void Game::initializeGame() {
	// Generate game
	time = 0;
	loop = 0;

	// Generate characters
	player = new Player(&map);
	player->add(this, OBSERVER_OFFSET);
	characters.push_back(player);

	Jenna* jenna = new Jenna(&map);
	jenna->add(this, OBSERVER_OFFSET + 1);
	characters.push_back(jenna);
	npcs.push_back(jenna);

	George* george = new George(&map);
	george->add(this, OBSERVER_OFFSET + 2);
	characters.push_back(george);
	npcs.push_back(george);

	xml_document doc;
	if (doc.load_file("files/save.xml") && !doc.child("GameData").child("Game").child("Characters").child("Baxter").empty()) { // TODO make a better check
		doc.load_file("files/base.xml");
		Magnus* magnus = new Magnus(&map);
		magnus->add(this, OBSERVER_OFFSET + 3);
		characters.push_back(magnus);
		npcs.push_back(magnus);

		Santos* santos = new Santos(&map);
		santos->add(this, OBSERVER_OFFSET + 4);
		characters.push_back(santos);
		npcs.push_back(santos);

		Baxter* baxter = new Baxter(&map);
		baxter->add(this, OBSERVER_OFFSET + 5);
		characters.push_back(baxter);
		npcs.push_back(baxter);

		Renard* renard = new Renard(&map);
		renard->add(this, OBSERVER_OFFSET + 6);
		characters.push_back(renard);
		npcs.push_back(renard);

		Paul* paul = new Paul(&map);
		paul->add(this, OBSERVER_OFFSET + 7);
		characters.push_back(paul);
		npcs.push_back(paul);

		Hilda* hilda = new Hilda(&map);
		hilda->add(this, OBSERVER_OFFSET + 8);
		characters.push_back(hilda);
		npcs.push_back(hilda);

		Tom* tom = new Tom(&map);
		tom->add(this, OBSERVER_OFFSET + 9);
		characters.push_back(tom);
		npcs.push_back(tom);

		Liz* liz = new Liz(&map);
		liz->add(this, OBSERVER_OFFSET + 10);
		characters.push_back(liz);
		npcs.push_back(liz);

		Willow* willow = new Willow(&map);
		willow->add(this, OBSERVER_OFFSET + 11);
		characters.push_back(willow);
		npcs.push_back(willow);

		Amelie* amelie = new Amelie(&map);
		amelie->add(this, OBSERVER_OFFSET + 12);
		characters.push_back(amelie);
		npcs.push_back(amelie);
	}
	else {
		doc.load_file("files/intro.xml");
	}

	// Generate map
	vector<Room*> rooms;
	vector<string> roomList = FileManager::getFileList("files/rooms");

	for (int i = 0; i < roomList.size(); i++) {
		FileDict roomFile = FileManager::readFromFile(roomList[i]);

		Room* room;
		if (roomFile.hasKey("codename"))
			room = new Room(i, roomFile.getValue("name"), roomFile.getValue("codename"), roomFile.getValue("text"), roomFile.getValues("adjacent"));
		else
			room = new Room(i, roomFile.getValue("name"), roomFile.getValue("text"), roomFile.getValues("adjacent"));
		rooms.push_back(room);
	}

	map = Map(rooms, this);

	xml_node load_package = doc.child("GameData").child("Game").child("Map");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		Room* thisRoom = map.getRoom(it->name());
		thisRoom->clearObjects();

		// Load objects
		vector<string*> objectNames;
		xml_node objects = it->child("Objects");

		for (xml_node_iterator ait = objects.begin(); ait != objects.end(); ++ait)
			objectNames.push_back(new string( ait->name()) );

		thisRoom->setObjectNames(objectNames);
		map.loadRoom(thisRoom);
	}

	// Generate characters
	load_package = doc.child("GameData").child("Game").child("Characters");
	int i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		string s = it->attribute("Room").value();
		Character* character = findCharacter(it->name());
		character->setCurrentRoom(map.getRoom(s));
	}

	// Load conversations
	load_package = doc.child("GameData").child("Game").child("Conversations");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it)
		conversations.push_back(Conversation(it->name(), it->attribute("room").value(), stoi(it->attribute("stage").value())));
}


bool Game::loadGame(string loadFile) {
	xml_document doc;
	if (!doc.load_file(("files/" + loadFile).c_str()))
		return false;

	// Load game
	xml_node load_package = doc.child("GameData").child("Game");
	time = load_package.attribute("Time").as_int();
	if (loop == 0)
		loop = load_package.attribute("Loop").as_int();

	// Load map
	load_package = doc.child("GameData").child("Game").child("Map");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		Room* thisRoom = map.getRoom( it->name() );
		thisRoom->clearObjects();

		// Load objects
		vector<string*> objectNames;
		xml_node node = it->child("Objects");

		for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
			objectNames.push_back(new string( ait->name()) );

		thisRoom->setObjectNames(objectNames);
		map.loadRoom(thisRoom);
	}

	// Load characters
	load_package = doc.child("GameData").child("Game").child("Characters");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		Character* thisCharacter = findCharacter(it->name());
		string room = it->attribute("Room").value();
		thisCharacter->setCurrentRoom(map.getRoom(room));

		// Load inventory
		xml_node node = it->child("Inventory");
		for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait) {
			obtainObject(ait->name(), thisCharacter);
		}

		// Load NPCs
		if (thisCharacter != player) {
			// Load atoms
			node = it->child("Atoms");
			((NPC*)thisCharacter)->setupWorld();
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait) {
				string s = ait->attribute("value").value();
				((NPC*)thisCharacter)->setCondition(ait->name(), (s == "true"));
			}

			// Load goals
			node = it->child("Goals");
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait) {
				__int64 val;
				std::stringstream sstr(string(ait->name()).substr(1, string::npos)); sstr >> val;
				bfield_t values = val;
				std::stringstream sstr2(ait->attribute("dontcare").value()); sstr2 >> val;
				bfield_t dontcare = val;
				string onetime = ait->attribute("onetime").value();
				int priority = stoi(ait->attribute("priority").value());
				((NPC*)thisCharacter)->addGoal(values, dontcare, (onetime == "true"), priority);
			}
		}
		// Load player
		else {
			// Load rumors
			node = it->child("Rumors");
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
				obtainAbstract(ait->name(), thisCharacter);

			// Load concepts
			node = it->child("Concepts");
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
				obtainAbstract(ait->name(), thisCharacter);
		}
	}

	// Load conversations
	load_package = doc.child("GameData").child("Game").child("Conversations");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it)
		conversations.push_back(Conversation( it->name(), it->attribute("room").value(), (strcmp(it->attribute("is_reaction").value(), "true") == 0),
			stoi(it->attribute("stage").value())));

	return true;
}


void Game::saveGame(string baseSave) {
	xml_document doc;
	if (!doc.load_file( ("files/" + baseSave).c_str() ))
		doc.load_file("files/intro.xml");
	xml_node node = doc.child("GameData").child("Game");

	// Save game
	node.attribute("Time").set_value(to_string(time).c_str());
	node.attribute("Loop").set_value(to_string(loop).c_str());

	// Save map
	node = doc.child("GameData").child("Game").child("Map");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Room* thisRoom = map.getRoom(it->name());

		// Save objects
		xml_node objects = it->child("Objects");
		objects.remove_children();
		for (int i = 0; i < thisRoom->getObjectNames().size(); i++) {
			objects.append_child((*thisRoom->getObjectNames()[i]).c_str());
		}
	}

	// Save characters
	node = doc.child("GameData").child("Game").child("Characters");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Character* thisCharacter = findCharacter(it->name());
		it->attribute("Room").set_value(thisCharacter->getCurrentRoom()->getCodename().c_str() );

		// Save inventory
		xml_node inventory = it->child("Inventory");
		inventory.remove_children();
		vector<Item*> itemList = thisCharacter->getItems();
		for (int i = 0; i < itemList.size(); i++)
			if (!inventory.child(itemList[i]->getCodename().c_str()))
				inventory.append_child(itemList[i]->getCodename().c_str());

		// Save NPCs
		if (thisCharacter != player) {
			xml_node atoms = it->child("Atoms");
			vector<string> atomList = ((NPC*)thisCharacter)->getAtomList();

			// Create and/or update each atom
			for (int i = 0; i < atomList.size(); i++) {
				if (!atoms.child(atomList[i].c_str())) {
					atoms.append_child(atomList[i].c_str());
					atoms.child(atomList[i].c_str()).append_attribute("value");
				}
				atoms.child(atomList[i].c_str()).attribute("value").set_value( thisCharacter->hasCondition(atomList[i]) );
			}

			// Goal list updated
			xml_node goals = it->child("Goals");
			vector<Goal> goalList = ((NPC*)thisCharacter)->getGoalList();	// TODO: maybe make this more pointer-oriented
			goals.remove_children();
			for (int i = 0; i < goalList.size(); i++) {
				string nodename = "a" + to_string(goalList[i].goal.values);
				goals.append_child(nodename.c_str());
				goals.child(nodename.c_str()).append_attribute("dontcare");
				goals.child(nodename.c_str()).append_attribute("onetime");
				goals.child(nodename.c_str()).append_attribute("priority");

				goals.child(nodename.c_str()).attribute("dontcare").set_value(goalList[i].goal.dontcare);
				goals.child(nodename.c_str()).attribute("onetime").set_value(goalList[i].onetime);
				goals.child(nodename.c_str()).attribute("priority").set_value(goalList[i].priority);
			}
		}

		// Save Player
		else {
			// Create and/or update each concept
			xml_node rumors = it->child("Rumors");
			vector<Concept*> rumorList = thisCharacter->getRumors();
			for (int i = 0; i < rumorList.size(); i++)
				if (!rumors.child(rumorList[i]->getCodename().c_str()))
					rumors.append_child(rumorList[i]->getCodename().c_str());

			xml_node concepts = it->child("Concepts");
			vector<Concept*> conceptList = thisCharacter->getConcepts();
			for (int i = 0; i < conceptList.size(); i++)
				if (!concepts.child(conceptList[i]->getCodename().c_str()))
					concepts.append_child(conceptList[i]->getCodename().c_str());
		}
	}

	// Save conversations
	node = doc.child("GameData").child("Game").child("Conversations");
	node.remove_children();
	for (int i = 0; i < conversations.size(); i++) {
		string convoName = conversations[i].getName();
		node.append_child(convoName.c_str());
		node.child(convoName.c_str()).append_attribute("stage");
		node.child(convoName.c_str()).append_attribute("room");
		node.child(convoName.c_str()).append_attribute("is_reaction");
		node.child(convoName.c_str()).attribute("stage").set_value(conversations[i].getStage());
		node.child(convoName.c_str()).attribute("room").set_value(conversations[i].getRoom().c_str());
		node.child(convoName.c_str()).attribute("is_reaction").set_value(conversations[i].getIsReaction());
	}

	doc.save_file("files/save.xml");
}


void Game::rewindGame() {
	// Clear NPCs
	for (int i = 0; i < npcs.size(); i++) {
		npcs[i]->clear();
	}
	player->rewind();

	conversations.clear();
	map.clearAllObjects();
	loop++;

	if (findCharacter("Baxter") == NULL) {
		Magnus* magnus = new Magnus(&map);
		magnus->add(this, OBSERVER_OFFSET + 3);
		characters.push_back(magnus);
		npcs.push_back(magnus);

		Santos* santos = new Santos(&map);
		santos->add(this, OBSERVER_OFFSET + 4);
		characters.push_back(santos);
		npcs.push_back(santos);

		Baxter* baxter = new Baxter(&map);
		baxter->add(this, OBSERVER_OFFSET + 5);
		characters.push_back(baxter);
		npcs.push_back(baxter);

		Renard* renard = new Renard(&map);
		renard->add(this, OBSERVER_OFFSET + 6);
		characters.push_back(renard);
		npcs.push_back(renard);

		Paul* paul = new Paul(&map);
		paul->add(this, OBSERVER_OFFSET + 7);
		characters.push_back(paul);
		npcs.push_back(paul);

		Hilda* hilda = new Hilda(&map);
		hilda->add(this, OBSERVER_OFFSET + 8);
		characters.push_back(hilda);
		npcs.push_back(hilda);

		Tom* tom = new Tom(&map);
		tom->add(this, OBSERVER_OFFSET + 9);
		characters.push_back(tom);
		npcs.push_back(tom);

		Liz* liz = new Liz(&map);
		liz->add(this, OBSERVER_OFFSET + 10);
		characters.push_back(liz);
		npcs.push_back(liz);

		Willow* willow = new Willow(&map);
		willow->add(this, OBSERVER_OFFSET + 11);
		characters.push_back(willow);
		npcs.push_back(willow);

		Amelie* amelie = new Amelie(&map);
		amelie->add(this, OBSERVER_OFFSET + 12);
		characters.push_back(amelie);
		npcs.push_back(amelie);
	}

	loadGame("base.xml");
	saveGame("base.xml");
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
		character->setStatus("obtaining " + object->getName() + ".");	// Update char's status
		break;
	}
}


void Game::characterAction(Character* character) {
	int id = character->getNotifyID();
	Character* target;
	Room* oldRoom;

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
		conversations.push_back( Conversation(character->getNotifyText(), character->getCurrentRoom()->getCodename(),
												character->getNotifyArgs()[1][0] == 'r') );
		break;

	case ouvir:
		for (int i = 0; i < conversations.size(); i++)
			if (conversations[i].participates(character->getNotifyText())) {
				conversations[i].addListener(character->getName());
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
				conversations.push_back(Conversation("intro2", "Runway"));
			}
		}
		
		break;
	}

}


void Game::advanceTime() {
	// Decide action
	for (int i = 0; i < npcs.size(); i++) {
		int action = npcs[i]->decideAction();
		if (action == conversar)
			npcs[i]->takeAction();
	}
	if (player->getAction() == ouvir)
		player->takeAction();

	// Conversations
	advanceConversations();

	// Order by priority
	PriorityVector<Character*> orderAction = PriorityVector<Character*>(vector<Character*>(), actionCompare);
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getAction() != conversar && characters[i]->getAction() != ouvir)
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
	saveGame("save.xml");
}


void Game::advanceConversations() {
	// Iterate through each current convo
	for (vector<Conversation>::iterator it = conversations.begin(); it != conversations.end(); it++) {
		// Try until a message shoots through
		while (1) {
			// If the convo is over
			bool endConvo = false;
			if (it->ended()) {
				string s = it->getName();
				conversations.erase(it);
				emitEvent(_evento_fim_conversa, vector<string>({ s }));
				break;
			}

			// Advance
			xml_node conversation = it->nextLine();

			Character* speaker = NULL;
			bool isNarrator = (string(conversation.name()) == "Narrator");
			if (!isNarrator) {
				speaker = findCharacter(conversation.name());
				// Test if valid
				// Is the speaker in the room?
				if (!speaker || speaker->getCurrentRoom()->getCodename() != it->getRoom())
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

				bool conditionMet = ( !checkTag && (isNarrator || speaker->hasCondition(cit->name())) ) || ( checkTag && it->hasTag(cit->name()) );
				string nao = cit->attribute("n").value();
				bool inverted = (nao == "n");
				
				if (addTag == "add_tag")
					it->addTag(cit->name());
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
				broadcastMessage( infoAtom, conversation.attribute("line").value(), "", *(it->getParticipants()), map.getRoom(it->getRoom()) );
			else
				speaker->sayLine( infoAtom, conversation.attribute("line").value(), *(it->getParticipants()) );

			// Lock every participant
			for (set<string>::iterator ait = it->getParticipants()->begin(); ait != it->getParticipants()->end(); ait++)
				findCharacter(*ait)->setInConversation(true);
			it->clearListeners();

			// End convo?
			if (endConvo)
				conversations.erase(it);
			break;
		}

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
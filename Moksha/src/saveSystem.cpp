#include "game.h"

// SAVE/LOAD --------------------------------------
void Game::initializeGame() {
	// Generate game
	time = 0;
	loop = 0;

	// Generate characters and scenario
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
			objectNames.push_back(new string(ait->name()));

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
		conversations.push_back(new Conversation(it->name(), it->attribute("room").value(), characters, stoi(it->attribute("stage").value())));
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
		Room* thisRoom = map.getRoom(it->name());
		thisRoom->clearObjects();

		// Load objects
		vector<string*> objectNames;
		xml_node node = it->child("Objects");

		for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
			objectNames.push_back(new string(ait->name()));

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
			thisCharacter->obtainObject(ait->name());
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
			if (node.begin() != node.end()) {
				((NPC*)thisCharacter)->clearGoals();
				for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait) {
					__int64 val;
					std::stringstream sstr(string(ait->name()).substr(1, string::npos)); sstr >> val;
					bfield_t values = val;
					if (ait->attribute("dontcare").as_string() != "") {
						std::stringstream sstr2(ait->attribute("dontcare").value()); sstr2 >> val;
					}
					else {
						// In case of error...
						if (loadFile == "save.xml") {
							player->printGameText("Save file corrupted. Restoring latest backup...");
							return loadGame("backupE.xml");
						} else if (loadFile == "backupE.xml")
							return loadGame("backupO.xml");
					}

					bfield_t dontcare = val;
					string onetime = ait->attribute("onetime").value();
					int priority = stoi(ait->attribute("priority").value());
					((NPC*)thisCharacter)->addGoal(values, dontcare, (onetime == "true"), priority);
				}
			}
			((NPC*)thisCharacter)->createPlan();
		}
		// Load player
		else {
			// Load rumors
			node = it->child("Rumors");
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
				thisCharacter->obtainAbstract(ait->name());

			// Load concepts
			node = it->child("Concepts");
			for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
				thisCharacter->obtainAbstract(ait->name());
		}
	}

	// Load conversations
	load_package = doc.child("GameData").child("Game").child("Conversations");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it)
		conversations.push_back(new Conversation(it->name(), it->attribute("room").value(), characters, 
								(strcmp(it->attribute("is_reaction").value(), "true") == 0), stoi(it->attribute("stage").value())));


	return true;
}


void Game::saveGame(string baseSave) {
	xml_document doc;
	if (!doc.load_file(("files/" + baseSave).c_str()))
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
		it->attribute("Room").set_value(thisCharacter->getCurrentRoom()->getCodename().c_str());

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
				atoms.child(atomList[i].c_str()).attribute("value").set_value(thisCharacter->hasCondition(atomList[i]));
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
		string convoName = conversations[i]->getName();
		node.append_child(convoName.c_str());
		node.child(convoName.c_str()).append_attribute("stage");
		node.child(convoName.c_str()).append_attribute("room");
		node.child(convoName.c_str()).append_attribute("is_reaction");
		node.child(convoName.c_str()).attribute("stage").set_value(conversations[i]->getStage());
		node.child(convoName.c_str()).attribute("room").set_value(conversations[i]->getRoom().c_str());
		node.child(convoName.c_str()).attribute("is_reaction").set_value(conversations[i]->getIsReaction());
	}

	doc.save_file("files/save.xml");
	// Backup file
	if (time % 5 == 0) {
		std::ifstream  src("files/save.xml"); 
		if (time % 10 == 0) {
			std::ofstream  dst("files/backupE.xml");
			dst << src.rdbuf();
		}
		else {
			std::ofstream  dst("files/backupO.xml");
			dst << src.rdbuf();
		}
	}
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

	// intro.xml to base.xml
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

	// Reload base.xml
	loadGame("base.xml");
	saveGame("base.xml");
}
#include "player.h"

Player::Player(Map* map) : Character(M, 5, 7) {
	name = "Elliot";
	this->mapp = map;

	FileDict fileErros = FileManager::readFromFile("files/errors.txt");
	noObjectError = fileErros.getValues("no object")[0];
	noActionError = fileErros.getValues("no action")[0];
	noRoomError = fileErros.getValues("no room")[0];
	noItemError = fileErros.getValues("no item")[0];
	noPersonError = fileErros.getValues("no person")[0];
	mindError = fileErros.getValues("mind theory")[0];

};

void Player::rewind() {
	inventory.clearItems();
	mindTheory.clear();
}


// ACOES -------------------------------------------------------------------------------------

void Player::interact(string action, string object) {
	// TODO: Maybe delete all the checks? They're already in receiveArgs

	// Objects
	if (getCurrentRoom()->hasObject(object)) {
		// Print answer
		string response = getCurrentRoom()->getObject(object)->getResponse(action);
		if (response.size() > 0) { printText(response); }
		else { printText(noActionError); }

		// Take action
		Character::interact(action, object);
	}

	// Object doesn't exist
	else
		printText(noObjectError);
}


bool Player::characterCheck(vector<string> args) {
	if (args.size() > 1 && names.count(args[1]) > 0)
		return true;
	printText(noPersonError);
	return false;
}

void Player::receiveArgs(vector<string> args) {
	// TODO: FIX THIS GARBAGE CODE
	currentAction = nulo;

	if (args[0].length() <= 1 || args[0] == "wait" || args[0] == "rest")
		currentAction = descansar;

	else if (args[0] == "move") {
		string destination = concatStrings(args, 1);
		// Room doesn't exist
		if (!mapp->hasRoomByName(destination)) {
			printText(noRoomError);
			return;
		}
		// Room isn't adjacent
		destination = mapp->getRoomByName(destination)->getCodename();
		if (!currentRoom->isRoomAdjacent(destination)) {
			printText(noRoomError);
			return;
		}
		// Move
		actionArgs = vector<string>({ destination });
		currentAction = mover;
		notify(avancar);
		return;
	}

	else if (args[0] == "mention") {
		// Person doesn't exist
		if (!(args.size() > 2 && names.count(args[1]) > 0)) {
			printText(noPersonError);
			return;
		}

		string topic = concatStrings(args, 2);
		// Person already heard this
		if (mindTheory.count(topic) && mindTheory.at(topic).find(args[1]) != mindTheory.at(topic).end()) {
			printText(mindError);
			return;
		}

		Concept* info = inventory.getInfoByName(topic);
		// Don't have this info/item
		if (info == NULL)
			printText(noItemError);
		// Mention
		else {
			mention(info->getCodename(), args[1]);
			currentAction = descansar;
		}
		return;
	}

	else if (args[0] == "attack")
		if (characterCheck(args))
			currentAction = atacar;
		else
			return;
	else if (args[0] == "leave" || args[0] == "put")
		if (args.size() > 1 && inventory.hasItem( concatStrings(args, 1) ))
			currentAction = deixar;
		else
			return;
	else if (args[0] == "listen" || args[0] == "overhear" || args[0] == "eavesdrop" || args[0] == "hear") {
		if (characterCheck(args))
			currentAction = ouvir;
		else
			return;
	}
	else if (args[0] == "see" || args[0] == "check" || args[0] == "look") {
		if (args.size() <= 0 || args[1] == "around" || args[1] == "room" || (args.size() > 2 && args[2] == "room")) {
			scan();	return;
		}
		else if (names.count(args[1]) > 0) {
			check(concatStrings(args, 1));	return;
		}
		
	}
	else if (args[0] == "scan") {
		scan();
		return;
	}

	if (currentAction == nulo) {	// Hasn't gotten an action yet
		// Not a valid action
		if (args.size() < 2) {
			printText(noActionError);
			return;
		}
		string objectName = concatStrings(args, 1);

		// Not an object
		if ( !getCurrentRoom()->hasObject(objectName) ) {
			printText(noObjectError);
			return;
		}
		// This object doesn't support this action
		else if (getCurrentRoom()->getObject(objectName)->getResponse(args[0]).size() <= 0) {
			printText(noActionError);
			return;
		}

		currentAction = interagir;
	}

	if (currentAction != interagir)
		args.erase(args.begin());

	actionArgs = args;
	notify(avancar);
}


// REACOES -----------------------------------------------------------------------

void Player::executeReaction(string topic, string phrase, string sender, bool shouldRespond) {
	if (topic == "busy")
		printText(sender + " is busy.");
	else if (sender != "")
		printText(sender + ": " + phrase);
	else if (phrase != "")
		printText(phrase);

	if (topic != "") {
		addToMind(topic, sender);
	}
}


void Player::receiveCheck(Character* checkTarget) {
	printText( *((NPC*)checkTarget)->getDescription() );
}


void Player::checkRoom(vector<Character*> charsInRoom) {
	// Adjacent rooms
	printText("Current room: " + getCurrentRoom()->getName() + "\n" + getCurrentRoom()->getInitialText() + "\nAdjacent rooms:");
	for (int i = 0; i < getCurrentRoom()->getAdjacentRoomCount(); i++)
		printText(mapp->getRoom(getCurrentRoom()->getAdjacentRoomCodename(i))->getName());

	// Objects in the room
	printText("Objects in the room: ");
	vector<Object*> objects = getCurrentRoom()->getObjects();
	if (objects.size() == 0)
		printText("There's no object here.");
	else {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->isVisible())
				printText(objects[i]->getName());
		}
	}
	updateRoom(charsInRoom);
}

void Player::updateRoom(vector<Character*> charsInRoom) {
	// Characters in the room
	for (int i = 0; i < charsInRoom.size(); i++) {
		if (charsInRoom[i]->getName() != name)
			if (charsInRoom[i]->isDead())
				printText("Oh, what the...?! " + charsInRoom[i]->getName() + " is dead!");
			else if (charsInRoom[i]->isUnconscious())
				printText("Oh, " + charsInRoom[i]->getName() + " is unconscious here!");
			else
				printText(charsInRoom[i]->getName() + " is in the room, " + charsInRoom[i]->getStatus());
	}
}



void Player::seeCharMoving(Character* person, Room* otherRoom, bool entering) {
	if (entering)
		printText(person->getName() + " entered the room, coming from the " + otherRoom->getName());
	else
		printText(person->getName() + " left the room, going to the " + otherRoom->getName());
}

// HELPER FUNCTIONS ----------------------------------------------------

bool Player::hasCondition(string info) {
	return inventory.hasConcept(info);
}



void Player::addToMind(string topic, string character) {
	// Add to stuff I know they know
	if (mindTheory.count(character)) {
		for (int i = 0; i < mindTheory.at(character).size(); i++)
			mindTheory.at(character).insert(topic);
	}
	else {
		string s[] = { topic };
		mindTheory.insert(pair<string, set<string>>(character, set<string>()));
		mindTheory[character].insert(topic);
	}
}
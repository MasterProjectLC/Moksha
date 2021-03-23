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


// ACOES -------------------------------------------------------------------------------------

void Player::interact(string action, string object) {
	// TODO: Maybe delete all the checks? They're already in receiveArgs

	// Objects
	if (getCurrentRoom()->hasObject(object)) {
		// Print answer
		vector<string> responses = getCurrentRoom()->getObject(object)->getResponses(action);
		if (responses.size() > 0) { printText(responses[0]); }
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
	if (args[0] == "move") {
		// TODO: FIX THIS
		string destinationName = concatStrings(args, 1);
		// Room doesn't exist or isn't adjacent
		if (mapp->hasRoomByName(destinationName) || !currentRoom->isRoomAdjacent(mapp->getRoomByName(destinationName)->getCodename()))
			printText(noRoomError);

		// Move
		else {
			actionArgs = vector<string>({ destinationName });
			currentAction = mover;
			notify(avancar);
		}
		return;
	}

	else if (args[0] == "mention")
		if (!(args.size() > 2 && names.count(args[2]) > 0)) {
			printText(noPersonError);
			return;
		}
		else if (mindTheory.count(args[2]) && mindTheory.at(args[2]).find(args[1]) != mindTheory.at(args[2]).end()) {
			printText(mindError);
			return;
		}
		else if (!inventory.hasConcept(args[1]) && !inventory.hasItem(args[1])) {
			printText(noItemError);
			return;
		}
		else
			currentAction = mencionar;

	else if (args[0] == "attack")
		if (characterCheck(args))
			currentAction = atacar;
		else
			return;
	else if ((args[0] == "listen" || args[0] == "overhear" || args[0] == "eavesdrop" || args[0] == "hear"))
		if (characterCheck(args))
			currentAction = ouvir;
		else
			return;
	else if ((args[0] == "see" || args[0] == "check" || args[0] == "look"))
		if (characterCheck(args))
			currentAction = checar;
		else
			return;

	else if (args[0] == "wait" || args[0] == "rest")
		currentAction = descansar;

	else {
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
		if (getCurrentRoom()->getObject(objectName)->getResponses(args[0]).size() <= 0) {
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
	printText(sender + ": " + phrase);
	if (topic != "" && !inventory.hasConcept(topic)) {
		inventory.addConcept(topic);
		addToMind(topic, sender);
	}
}


void Player::receiveCheck(Character* checkTarget) {
	printText( *((NPC*)checkTarget)->getDescription() );
}


void Player::checkRoom(vector<Character*> charsInRoom) {
	// Salas anexas
	printText("Current room: " + getCurrentRoom()->getName() + "\n" + getCurrentRoom()->getInitialText() + "\nAdjacent rooms:");
	for (int i = 0; i < getCurrentRoom()->getAdjacentRoomCount(); i++)
		printText(mapp->getRoom(getCurrentRoom()->getAdjacentRoomCodename(i))->getName());

	// Objetos na sala
	printText("Objects in the room: ");
	vector<Object> objects = getCurrentRoom()->getObjects();
	if (objects.size() == 0)
		printText("There's no object here.");
	else {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].isVisible())
				printText(objects[i].getName());
		}
	}
}

void Player::updateRoom(vector<Character*> charsInRoom) {
	// Characters in the room na sala
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
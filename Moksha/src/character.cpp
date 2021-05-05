#include "character.h"

void Character::takeAction(int action, vector<string> args) {
	// Locked in conversation
	if (inConversation()) {
		setInConversation(false);
		return;
	}

	string zusammenArgs = "";
	if (args.size() >= 1)
		zusammenArgs = concatStrings(args, 0);

	switch(action) {
		case mencionar:
			mention(concatStrings(args, 2), set<string>({ args.at(1) }));
			break;

		case mover:
			move(zusammenArgs);
			break;

		case atacar:
			attack(zusammenArgs);
			break;

		case interagir:
			if (args.size() >= 2)
				interact(args.at(0), concatStrings(args, 1));
			break;

		case tempo:
			notify(tempo);
			break;

		case ouvir:
			listen(zusammenArgs);
			break;

		case pegar:
			take(zusammenArgs);
			break;

		case deixar:
			leave(zusammenArgs);
			break;

		case checar:
			check(zusammenArgs);
			break;

		case conversar:
			talk(zusammenArgs);
			break;

		case acaoNula:
			voidAction(zusammenArgs);
			break;

		default:
			rest();
			break;
	}

}

// ACTIONS ======================================

void Character::move(string str) {
	status = "entering a room.";
	// Neighbours see char leaving
	for (vector<Character*>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
		(*it)->seeCharMoving(this, mapp->getRoom(str), false);

	// Char enters the room
	string oldRoom = str;
	setCurrentRoom(mapp->getRoom(str));

	// Update neighbours
	notify(atualizar_vizinhos);

	// Neighbours see char entering
	for (vector<Character*>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
		(*it)->seeCharMoving(this, mapp->getRoom(str), true);
}

void Character::move(Room* room) {
	move(room->getCodename());
}

void Character::mention(string obj, string receiver) {
	mention( obj, set<string>({ receiver }) );
}

void Character::mention(string obj, set<string> receivers) {
	for (vector<Character*>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
		if (receivers.count((*it)->getName()))
			(*it)->executeReaction(obj, "", this, true);
}

void Character::attack(string targetName) {
	status = "attacking " + targetName + "!";
	Character* target = findNeighbour(getNotifyText());
	if (target->getCurrentRoom() == getCurrentRoom())
		if (target->beAttacked(this))
			broadcastEvent(vector<string>({ "attack", target->getName() }));
}

bool Character::beAttacked(Character* attacker) {
	if (attacker->getStrength() >= strength)
		unconscious = true;
	else {
		broadcastEvent(vector<string>({ "attack", attacker->getName() }));
		attacker->beAttacked(this);
	}
	return unconscious;
};

void Character::leave(string itemName) {
	status = "leaving " + itemName + ".";
	notifyArgs.clear();
	notifyArgs.push_back(itemName);
	notify(deixar);

	Item* item = getItem(itemName);
	string codename = item->getCodename();
	if (item != NULL && item->isActionValid("leave")) {
		FileDict fileDict = FileManager::readFromFile("files/objects/" + codename + ".txt");
		getCurrentRoom()->addObject(new Object(fileDict));
		removeItem(codename);
	}
}

void Character::listen(string target) {
	status = "eavesdropping " + target + ".";
	notifyArgs.clear();
	notifyArgs.push_back(target);
	notify(ouvir);
}

void Character::check(string targetName) {
	status = "checking " + targetName + ".";
	Character* target = findNeighbour(targetName);
	receiveCheck(target);
}

void Character::scan() {
	status = "checking the room.";
	checkRoom(neighbours);
}

void Character::say(string topic, string str, vector<Character*> receivers) {
	for (vector<Character*>::iterator it = receivers.begin(); it != receivers.end(); it++)
		if ((*it)->getCurrentRoom() == currentRoom)
			(*it)->executeReaction(topic, str, this, false);
}

void Character::rest() {
	status = "doing nothing.";
}

void Character::talk(string convo) {
	talk(convo, false);
}

void Character::talk(string convo, bool isReaction) {
	status = "talking.";
	notifyArgs.clear();
	notifyArgs.push_back(convo);
	if (isReaction)
		notifyArgs.push_back("r");
	else
		notifyArgs.push_back("c");
	notify(conversar);
}

void Character::take(string objectName) {
	Object* object = currentRoom->getObject(objectName);
	obtainObject(object->getCodename());					// Give object to character
	setStatus("obtaining " + object->getName() + ".");		// Update char's status
	getCurrentRoom()->removeObject(object);					// Remove object from the room
}

void Character::voidAction(string actionStatus) {
	status = actionStatus;
}

void Character::interact(string action, string objectName) {
	Object* object = getCurrentRoom()->getObject(objectName);
	int actionID = object->returnAction(action);
	takeAction(actionID, object->getArgs());
}

// HELPER ====================================

void Character::broadcastEvent(vector<string> args) {
	for (int i = 0; i < neighbours.size(); i++)
		if (neighbours[i] != this)
			neighbours[i]->receiveEvent(args);
}

void Character::checkRoom(vector<Character*> peopleInRoom) { 
	setNeighbours(peopleInRoom);
	checkRoomParticular(peopleInRoom);
}

void Character::seeCharMoving(Character* person, Room* otherRoom, bool entering) {
	seeCharMovingParticular(person, otherRoom, entering);
	if (entering)
		neighbours.push_back(person);
	else
		for (vector<Character*>::iterator it = neighbours.begin(); it != neighbours.end(); it++)
			if ((*it)->getName() == person->getName()) {
				neighbours.erase(it);
				break;
			}
}


// SETTERS ETC =================================================

void Character::setNeighbours(vector<Character*> neighbours) {
	this->neighbours = neighbours;
	for (vector<Character*>::iterator it = this->neighbours.begin(); it != this->neighbours.end(); it++)
		if ((*it)->getName() == name) {
			this->neighbours.erase(it);
			break;
		}
}


void Character::obtainObject(string codename) {
	FileDict filedict = FileManager::readFromFile("files/items/" + codename + ".txt");
	vector<string> actionVec = filedict.getValues("actions");
	set<string> actionSet = set<string>();
	for (int i = 0; i < actionVec.size(); i++)
		actionSet.insert(actionVec[i]);

	if (filedict.hasKey("codename"))
		addItem(filedict.getValues("name")[0], filedict.getValues("codename")[0], "", actionSet);
	else
		addItem(filedict.getValues("name")[0], filedict.getValues("name")[0], "", actionSet);
}


void Character::obtainAbstract(string codename) {
	FileDict filedict = FileManager::readFromFile("files/abstract/" + codename + ".txt");

	if (filedict.hasKey("codename"))
		addAbstract(filedict.getValues("name")[0], filedict.getValues("codename")[0], filedict.getValues("description")[0], filedict.getValues("type")[0][0]);
	else
		addAbstract(filedict.getValues("name")[0], filedict.getValues("name")[0], filedict.getValues("description")[0], filedict.getValues("type")[0][0]);
}

// GETTER =====================================

bool Character::isActionValid(int action) {
	if (basicActions.count(action) > 0)
		return true;

	for (int i = 0; i < getItems().size(); i++)
		if (getItems()[i]->isActionValid(action))
			return true;

	return false;
}

bool Character::getStatusEffect(string key) {
	if (!statusEffects.hasKey(key))
		return false;
	return statusEffects.getValues(key); 
}


Character* Character::findNeighbour(string name) {
	for (vector<Character*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
		if ((*it)->getName() == name)
			return (*it);
	}

	return NULL;
}
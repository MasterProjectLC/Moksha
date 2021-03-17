#include "character.h"

void Character::takeAction(int action, vector<string> args) {
	// Trancado em conversa
	if (inConversation()) {
		setInConversation(false);
		return;
	}

	string zusammenArgs = "";
	if (args.size() >= 1)
		zusammenArgs = concatStrings(args, 0);

	switch(action) {
		case mencionar:
			mention(args.at(1), set<string>({ args.at(2) }));
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

		case ouvir:
			listen(zusammenArgs);
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


void Character::printText(string str) {
	notifyText = str;
	notify(imprimir);
}

void Character::move(string str) {
	notifyText = str;
	notify(mover);
}

void Character::move(Room* room) {
	status = "entering the room.";
	move(room->getName());
}

void Character::mention(string obj, string receiver) {
	mention( obj, set<string>({ receiver }) );
}

void Character::mention(string obj, set<string> receivers) {
	if (inventory.hasConcept(obj) || inventory.hasItem(obj)) {
		notifyText = obj;
		notifyTargets = receivers;
		notify(mencionar);
	}
}

void Character::attack(string target) {
	status = "attacking " + target + "!";
	notifyText = target;
	notify(atacar);
}

void Character::listen(string target) {
	status = "eavesdropping " + target + ".";
	notifyText = target;
	notify(ouvir);
}

void Character::check(string target) {
	status = "checking " + target + ".";
	notifyText = target;
	notify(checar);
}

void Character::say(string topic, string str, set<string> receivers) {
	notifyText = topic + "|" + str;
	notifyTargets = receivers;
	notify(falar);
}

void Character::rest() {
	status = "doing nothing.";
	notify(descansar);
}

void Character::talk(string convo) {
	status = "talking.";
	notifyText = convo;
	notify(conversar);
}

void Character::voidAction(string actionStatus) {
	status = actionStatus;
	notify(descansar);
}

void Character::interact(string action, string object) {
	if (getCurrentRoom()->hasObject(object))
		getCurrentRoom()->getObject(object)->takeAction(action, name);
}

bool Character::isActionValid(int action) {
	if (basicActions.count(action) > 0)
		return true;

	for (int i = 0; i < getItems().size(); i++)
		if (getItems()[i].isActionValid(action))
			return true;

	return false;
}

bool Character::getStatusEffect(string key) {
	if (!statusEffects.hasKey(key))
		return false;
	return statusEffects.getValues(key); 
}
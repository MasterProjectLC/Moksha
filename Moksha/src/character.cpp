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

		case ouvir:
			listen(zusammenArgs);
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


void Character::printText(string str) {
	notifyArgs.clear();
	notifyArgs.push_back(str);
	notify(imprimir);
}

void Character::move(string str) {
	notifyArgs.clear();
	notifyArgs.push_back(str);
	notify(mover);
}

void Character::move(Room* room) {
	status = "entering the room.";
	move(room->getCodename());
}

void Character::mention(string obj, string receiver) {
	mention( obj, set<string>({ receiver }) );
}

void Character::mention(string obj, set<string> receivers) {
	notifyArgs.clear();
	notifyArgs.push_back(obj);
	notifyTargets = receivers;
	notify(mencionar);
}

void Character::attack(string target) {
	status = "attacking " + target + "!";
	notifyArgs.clear();
	notifyArgs.push_back(target);
	notify(atacar);
}

void Character::leave(string target) {
	status = "leaving " + target + ".";
	notifyArgs.clear();
	notifyArgs.push_back(target);
	notify(deixar);
}

void Character::listen(string target) {
	status = "eavesdropping " + target + ".";
	notifyArgs.clear();
	notifyArgs.push_back(target);
	notify(ouvir);
}

void Character::check(string target) {
	status = "checking " + target + ".";
	notifyArgs.clear();
	notifyArgs.push_back(target);
	notify(checar);
}

void Character::scan() {
	status = "checking the room.";
	notify(sondar);
}

void Character::say(string topic, string str, set<string> receivers) {
	notifyArgs.clear();
	notifyArgs.push_back(topic);
	notifyArgs.push_back(str);
	notifyTargets = receivers;
	notify(falar);
}

void Character::rest() {
	status = "doing nothing.";
	notify(descansar);
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
		if (getItems()[i]->isActionValid(action))
			return true;

	return false;
}

bool Character::getStatusEffect(string key) {
	if (!statusEffects.hasKey(key))
		return false;
	return statusEffects.getValues(key); 
}
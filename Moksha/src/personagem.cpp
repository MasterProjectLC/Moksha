#include "personagem.h"

void Personagem::takeAction(int action, vector<string> args) {
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
			status = "entering the room.";
			break;

		case atacar:
			attack(zusammenArgs);
			status = "attacking " + zusammenArgs + "!";
			break;

		case interagir:
			if (args.size() >= 2)
				interact(args.at(0), concatStrings(args, 1));
			break;

		case ouvir:
			listen(zusammenArgs);
			status = "eavesdropping " + zusammenArgs + ".";
			break;

		case conversar:
			talk(zusammenArgs);
			status = "talking.";
			break;

		default:
			rest();
			status = "doing nothing.";
			break;
	}
}


void Personagem::printText(string str) {
	notifyText = str;
	notify(imprimir);
}

void Personagem::move(string str) {
	notifyText = str;
	notify(mover);
}

void Personagem::move(Sala sala) {
	move(sala.getNome());
}

void Personagem::mention(string obj, string receiver) {
	mention( obj, set<string>({ receiver }) );
}

void Personagem::mention(string obj, set<string> receivers) {
	if (inventory.hasConcept(obj) || inventory.hasItem(obj)) {
		notifyText = obj;
		notifyTargets = receivers;
		notify(mencionar);
	}
}

void Personagem::attack(string target) {
	notifyText = target;
	notify(atacar);
}

void Personagem::listen(string target) {
	notifyText = target;
	notify(ouvir);
}

void Personagem::say(string topico, string str, set<string> receivers) {
	notifyText = topico + "|" + str;
	notifyTargets = receivers;
	notify(falar);
}

void Personagem::rest() {
	notify(descansar);
}

void Personagem::talk(string convo) {
	notifyText = convo;
	notify(conversar);
}

void Personagem::interact(string action, string object) {
	if (getCurrentRoom()->hasObject(object))
		getCurrentRoom()->getObject(object)->takeAction(action, name);
}

bool Personagem::isActionValid(int action) {
	if (basicActions.count(action) > 0)
		return true;

	for (int i = 0; i < getInventory().size(); i++)
		if (getInventory()[i].isActionValid(action))
			return true;

	return false;
}
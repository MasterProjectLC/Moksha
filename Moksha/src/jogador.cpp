#include "jogador.h"

Jogador::Jogador() : Personagem(M, 2, 2) {
	name = "Elliot";

	FileDict fileErros = FileManager::readFromFile("files/errors.txt");
	noObjectError = fileErros.getValues("no object")[0];
	noActionError = fileErros.getValues("no action")[0];
	noRoomError = fileErros.getValues("no room")[0];
	noItemError = fileErros.getValues("no item")[0];
	mindError = fileErros.getValues("mind theory")[0];

};


// ACOES -------------------------------------------------------------------------------------

void Jogador::mention(string topic, string person) {
	if (mindTheory.count(person) && mindTheory.at(person).find(topic) != mindTheory.at(person).end()) {
		printText(mindError);
		return;
	}

	if (!inventory.hasConcept(topic) && !inventory.hasItem(topic)) {
		printText(noItemError);
		return;
	}

	Personagem::mention(topic, set<string>({ person }));;
}


void Jogador::move(string location) {
	if (getCurrentRoom()->isSalaAnexa(location))
		Personagem::move(location);
	else
		printText(noRoomError);
}


void Jogador::interact(string acao, string object) {
	// Objetos
	if (getCurrentRoom()->hasObject(object)) {
		Object* objetoAqui = getCurrentRoom()->getObject(object);

		// Imprimir resposta
		vector<string> responses = objetoAqui->getResponses(acao);
		if (responses.size() > 0) { printText(responses[0]); }
		else { printText(noActionError); }

		// Tomar ação
		Personagem::interact(acao, object);
	}

	// Objeto não existe
	else
		printText(noObjectError);
}


void Jogador::receberArgs(vector<string> args) {
	if (args[0] == "move")
		currentAction = mover;
	else if (args[0] == "wait" || args[0] == "rest")
		currentAction = descansar;
	else if (args[0] == "mention")
		currentAction = mencionar;
	else if (args[0] == "attack" && args.size() > 1 && names.count(args[1]) > 0)
		currentAction = atacar;
	else if ((args[0] == "listen" || args[0] == "overhear" || args[0] == "eavesdrop" || args[0] == "hear") && args.size() > 1 && names.count(args[1]) > 0)
		currentAction = ouvir;
	else if ((args[0] == "see" || args[0] == "check" || args[0] == "look") && args.size() > 1 && names.count(args[1]) > 0)
		currentAction = checar;
	else
		currentAction = interagir;

	if (currentAction != interagir)
		args.erase(args.begin());

	actionArgs = args;
	notify(avancar);
}


// REACOES -----------------------------------------------------------------------

void Jogador::executeReaction(string topic, string phrase, string sender, bool shouldRespond) {
	printText(sender + ": " + phrase);
	if (topic != "" && !inventory.hasConcept(topic)) {
		inventory.addConcept(topic);
		addToMind(topic, sender);
	}
}


void Jogador::receiveCheck(Personagem* checkTarget) {
	printText( *((NPC*)checkTarget)->getDescription() );
}


void Jogador::checkRoom(vector<Personagem*> charsInRoom) {
	// Salas anexas
	printText("Current room: " + getCurrentRoom()->getName() + "\n" + getCurrentRoom()->getInitialText() + "\nAdjacent rooms:");
	for (int i = 0; i < getCurrentRoom()->getSalaAnexaCount(); i++)
		printText(getCurrentRoom()->getAdjacentRoomName(i));

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

void Jogador::updateRoom(vector<Personagem*> charsInRoom) {
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



void Jogador::seeCharMoving(Personagem* pessoa, string outraSala, bool entrando) {
	if (entrando)
		printText(pessoa->getName() + " entered the room, coming from the " + outraSala);
	else
		printText(pessoa->getName() + " left the room, going to the " + outraSala);
}

// HELPER FUNCTIONS ----------------------------------------------------

bool Jogador::hasCondition(string info) {
	return inventory.hasConcept(info);
}



void Jogador::addToMind(string topic, string character) {
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
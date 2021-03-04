#include "jogador.h"

Jogador::Jogador() : Personagem(M, 2, 2) {
	nome = "Elliot";

	FileDict fileErros = FileManager::readFromFile("files/errors.txt");
	erroSemObjeto = fileErros.getValues("no object")[0];
	erroSemAcao = fileErros.getValues("no action")[0];
	erroSemSala = fileErros.getValues("no room")[0];
	erroSemItem = fileErros.getValues("no item")[0];
	erroMente = fileErros.getValues("mind theory")[0];

};


// ACOES -------------------------------------------------------------------------------------

void Jogador::mention(string topic, string person) {
	if (mindTheory.count(person) && mindTheory.at(person).find(topic) != mindTheory.at(person).end()) {
		printText(erroMente);
		return;
	}

	if (!inventario.temConceito(topic) && !inventario.temItem(topic)) {
		printText(erroSemItem);
		return;
	}

	Personagem::mention(topic, set<string>({ person }));;
}


void Jogador::move(string location) {
	if (getSalaAtual()->isSalaAnexa(location))
		Personagem::move(location);
	else
		printText(erroSemSala);
}


void Jogador::interact(string acao, string object) {
	// Objetos
	if (getSalaAtual()->hasObject(object)) {
		Object* objetoAqui = getSalaAtual()->getObject(object);

		// Imprimir resposta
		vector<string> responses = objetoAqui->getResponses(acao);
		if (responses.size() > 0) { printText(responses[0]); }
		else { printText(erroSemAcao); }

		// Tomar ação
		Personagem::interact(acao, object);
	}

	// Objeto não existe
	else
		printText(erroSemObjeto);
}


void Jogador::receberArgs(vector<string> args) {
	int action;
	if (args[0] == "move")
		action = mover;
	else if (args[0] == "wait" || args[0] == "rest")
		action = descansar;
	else if (args[0] == "mention")
		action = mencionar;
	else if (args[0] == "attack")
		action = atacar;
	else {
		action = interagir;
	}

	if (action != interagir)
		args.erase(args.begin());
	takeAction(action, args);
}


// REACOES -----------------------------------------------------------------------

void Jogador::executeReaction(string topico, string frase, string remetente) {
	printText(remetente + ": " + frase);
	addToMind(topico, remetente);
}


void Jogador::checkRoom(vector<Personagem*> charsInRoom) {
	// Salas anexas
	printText("Current room: " + getSalaAtual()->getNome() + "\n" + getSalaAtual()->getTextoInicial() + "\nAdjacent rooms:");
	for (int i = 0; i < getSalaAtual()->getSalaAnexaCount(); i++)
		printText(getSalaAtual()->getSalaAnexaNome(i));

	// Objetos na sala
	printText("Objects in the room: ");
	vector<Object> objects = getSalaAtual()->getObjects();
	if (objects.size() == 0)
		printText("There's no object here.");
	else {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i].isVisible())
				printText(objects[i].getName());
		}
	}

	updateRoom(charsInRoom);
}

void Jogador::updateRoom(vector<Personagem*> charsInRoom) {
	// Characters in the room na sala
	for (int i = 0; i < charsInRoom.size(); i++) {
		if (charsInRoom[i]->getNome() != nome)
			if (!charsInRoom[i]->isUnconscious())
				printText(charsInRoom[i]->getNome() + " is in the room, " + charsInRoom[i]->getStatus());
			else
				printText("Oh, " + charsInRoom[i]->getNome() + " is unconscious here!");
	}
}



void Jogador::seeCharMoving(Personagem* pessoa, string outraSala, bool entrando) {
	if (entrando)
		printText(pessoa->getNome() + " entered the room, coming from the " + outraSala);
	else
		printText(pessoa->getNome() + " left the room, going to the " + outraSala);
}

// HELPER FUNCTIONS ----------------------------------------------------

bool Jogador::hasCondition(string info) {
	return inventario.temConceito(info);
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
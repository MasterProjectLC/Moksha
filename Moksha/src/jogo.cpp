#include "jogo.h"
#include <iostream>

Jogo::Jogo() {
	player = Jogador();
	player.add(this, OBSERVER_OFFSET);
	characters.push_back(&player);

	Baxter* baxter = new Baxter(&mapa);
	baxter->add(this, OBSERVER_OFFSET + 1);
	characters.push_back(baxter);
	npcs.push_back(baxter);

	Hilda* hilda = new Hilda(&mapa);
	hilda->add(this, OBSERVER_OFFSET + 2);
	characters.push_back(hilda);
	npcs.push_back(hilda);

	Santos* santos = new Santos(&mapa);
	santos->add(this, OBSERVER_OFFSET + 3);
	characters.push_back(santos);
	npcs.push_back(santos);

	Jenna* jenna = new Jenna(&mapa);
	jenna->add(this, OBSERVER_OFFSET + 4);
	characters.push_back(jenna);
	npcs.push_back(jenna);
}


void Jogo::setup() {
	initializeGame();
	if (!loadGame()) 
		for (int i = 0; i < npcs.size(); i++)
			npcs[i]->setupPlans();
}


// SAVE/LOAD --------------------------------------
void Jogo::initializeGame() {
	// Gerar jogo
	time = 1;
	loop = 1;

	xml_document doc;
	if (!doc.load_file("files/base.xml"))
		throw invalid_argument("ERROR INITIALIZING GAME - BASE.XML COULD NOT BE FOUND");

	// Gerar mapa
	vector<string> salaLista = FileManager::getFileList("files/rooms");
	vector<Sala*> salas;

	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = FileManager::readFromFile(salaLista[i]);

		Sala* room = new Sala(i, fileSala.getValue("name"), fileSala.getValue("text"),
			fileSala.getValues("adjacent"), fileSala.getValues("objects"));
		salas.push_back(room);
	}

	mapa = Mapa(salas, this);

	// Generate characters
	xml_node load_package = doc.child("GameData").child("Game").child("Characters");
	int i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		string s = it->attribute("Room").value();
		Personagem* personagem = findCharacter(it->name());
		personagem->setSalaAtual(mapa.getSala(s));
	}
}


bool Jogo::loadGame() {
	xml_document doc;
	if (!doc.load_file("files/save.xml"))
		return false;

	// Load game
	xml_node load_package = doc.child("GameData").child("Game");
	time = load_package.attribute("Time").as_int();
	loop = load_package.attribute("Loop").as_int();

	// Load map
	load_package = doc.child("GameData").child("Game").child("Map");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		Sala* estaSala = mapa.getSala(it->attribute("Name").value());
		estaSala->limparObjetos();

		// Load objects
		vector<string> objetoNomes;
		xml_node objetos = it->child("Objects");

		for (xml_node_iterator ait = objetos.begin(); ait != objetos.end(); ++ait)
			objetoNomes.push_back(ait->name());

		estaSala->setObjetoNomes(objetoNomes);
		mapa.carregarSala(estaSala);
	}

	// Load characters
	load_package = doc.child("GameData").child("Game").child("Characters");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		string s = it->attribute("Room").value();
		Personagem* thisCharacter = findCharacter(it->name());
		thisCharacter->setSalaAtual(mapa.getSala(s));

		// Load inventory
		xml_node inventario = it->child("Inventory");
		for (xml_node_iterator ait = inventario.begin(); ait != inventario.end(); ++ait) {
			obtainObject(ait->name(), thisCharacter);
		}

		// Load atoms
		if (thisCharacter != &player) {
			((NPC*)thisCharacter)->setupPlans();
			xml_node atoms = it->child("Atoms");
			for (xml_node_iterator ait = atoms.begin(); ait != atoms.end(); ++ait) {
				string s = ait->attribute("value").value();
				((NPC*)thisCharacter)->setCondition(ait->name(), (s == "true"));
			}
		}
	}

	// Load conversations
	load_package = doc.child("GameData").child("Game").child("Conversations");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		conversations.push_back(Conversa( it->name(), it->attribute("room").value(), stoi(it->attribute("stage").value()) ));
	}

	return true;
}


void Jogo::saveGame() {
	xml_document doc;
	if (!doc.load_file("files/save.xml"))
		doc.load_file("files/base.xml");
	xml_node node = doc.child("GameData").child("Game");

	// Save game
	node.attribute("Time").set_value(to_string(time).c_str());
	node.attribute("Loop").set_value(to_string(loop).c_str());

	// Save map
	node = doc.child("GameData").child("Game").child("Map");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Sala* thisSala = mapa.getSala(it->attribute("Name").value());

		// Save objects
		xml_node objetos = it->child("Objects");
		objetos.remove_children();
		for (int i = 0; i < thisSala->getObjetoNomes().size(); i++) {
			objetos.append_child(thisSala->getObjetoNomes()[i].c_str());
		}
	}

	// Save characters
	node = doc.child("GameData").child("Game").child("Characters");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Personagem* thisCharacter = findCharacter(it->name());
		it->attribute("Room").set_value(thisCharacter->getSalaAtual()->getNome().c_str() );

		// Save inventory
		xml_node inventario = it->child("Inventory");
		inventario.remove_children();
		vector<Item> itemList = thisCharacter->getInventario();
		for (int i = 0; i < itemList.size(); i++)
			inventario.append_child(itemList[i].getNome().c_str());

		// Save atoms
		if (thisCharacter != &player) {
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
		}
	}

	// Save conversations
	node = doc.child("GameData").child("Game").child("Conversations");
	for (int i = 0; i < conversations.size(); i++) {
		string convoName = conversations[i].getName();

		if (!node.child(convoName.c_str())) {
			node.append_child(convoName.c_str());
			node.child(convoName.c_str()).append_attribute("stage");
			node.child(convoName.c_str()).append_attribute("room");
		}

		node.child(convoName.c_str()).attribute("stage").set_value(conversations[i].getStage());
		node.child(convoName.c_str()).attribute("room").set_value(conversations[i].getRoom().c_str());
	}

	doc.save_file("files/save.xml");
}


// UPDATE --------------------------------------------------------
void Jogo::update(int id) {
	// Objeto
	if (id < OBSERVER_OFFSET) {
		objectAction(mapa.getObjeto(id));
	}

	// Personagem
	else {
		characterAction(characters[id - OBSERVER_OFFSET]);
	}
}


void Jogo::objectAction(Objeto* objeto) {
	// Obtain
	switch (objeto->getNotifyID()) {
	case objeto->obter:
		obtainObject(objeto->getName(), findCharacter(objeto->getUser()));				// Give object to character
		findCharacter(objeto->getUser())->getSalaAtual()->removeObjeto(*objeto);		// Remove object from the room

		if (player.getNome() == objeto->getUser())										// Player char - edge case
			advanceTime();
		break;
	}
}


void Jogo::characterAction(Personagem* personagem) {
	int id = personagem->getNotifyID();
	string antigaSala;

	switch (id) {
	case imprimir:
		printText(personagem->getNotifyText());
		break;

	case mover:
		// Pessoas dentro da sala veem pessoa saindo
		for (int i = 0; i < characters.size(); i++)
			if (characters[i] != personagem && characters[i]->getSalaAtual() == personagem->getSalaAtual())
				characters[i]->seeCharMoving(personagem, personagem->getNotifyText(), false);

		antigaSala = personagem->getSalaAtual()->getNome();
		personagem->setSalaAtual(moveRoom(personagem->getSalaAtual(), personagem->getNotifyText()));
		personagem->checkRoom(getPeopleInRoom(personagem->getSalaAtual()));

		// Pessoas dentro da sala veem pessoa entrando
		for (int i = 0; i < characters.size(); i++)
			if (characters[i] != personagem && characters[i]->getSalaAtual() == personagem->getSalaAtual())
				characters[i]->seeCharMoving(personagem, antigaSala, true);
		break;

	case mencionar:
		for (int i = 0; i < characters.size(); i++) {
			if (personagem->getNotifyTargets().count(characters[i]->getNome()) && characters[i]->getSalaAtual() == personagem->getSalaAtual()) {
				characters[i]->executeReaction(personagem->getNotifyText(), "", personagem->getNome());
				break;
			}
		}
		break;

	case falar:
		for (int i = 0; i < characters.size(); i++) {
			if (personagem->getNotifyTargets().count(characters[i]->getNome()) && characters[i]->getSalaAtual() == personagem->getSalaAtual()) {
				vector<string> args = splitString(personagem->getNotifyText(), '|');
				characters[i]->executeReaction(args[0], args[1], personagem->getNome());
				break;
			}
		}
		break;

	case conversar:
		conversations.push_back(Conversa(personagem->getNotifyText(), personagem->getSalaAtual()->getNome()));
		break;

	case descansar:
		break;

	case atacar:
		Personagem* vitima = findCharacter(personagem->getNotifyText());
		if (vitima->getSalaAtual() == personagem->getSalaAtual())
			vitima->serAtacado(personagem);
		break;
	}

	if (id != imprimir && personagem == &player)
		advanceTime();

}


void Jogo::advanceTime() {
	// Decidir Ação
	for (int i = 0; i < npcs.size(); i++) {
		if (npcs[i]->decideAction() == conversar)
			npcs[i]->takeAction();
	}

	// Conversas
	advanceConversations();

	// Ordenar por prioridade
	PriorityVector<NPC*> orderAction = PriorityVector<NPC*>(vector<NPC*>(), actionCompare);
	for (int i = 0; i < npcs.size(); i++) {
		if (npcs[i]->getAction() != conversar)
			orderAction.push(npcs[i]);
	}

	// Tomar Ação
	player.setInConversation(false);
	while (!orderAction.empty()) {
		(*orderAction.highest())->takeAction();
		orderAction.pop();
	}

	// Jogo
	time++;
	saveGame();
}


void Jogo::advanceConversations() {
	// Iterar por cada conversa acontecendo
	for (vector<Conversa>::iterator it = conversations.begin(); it != conversations.end(); it++) {
		// Tentar até uma mensagem passar
		while (1) {
			// Se conversa acabou
			if (it->ended()) {
				conversations.erase(it);
				break;
			}

			// Avancar
			bool valid = true;
			xml_node conversation = it->nextLine();

			string n = conversation.name();
			if (n == "Narrator") {
				if (it->getParticipants()->count(player.getNome()) && player.getSalaAtual()->getNome() == it->getRoom()) {	// Edge case: narrator
					printText(conversation.attribute("line").value());
				}
				break;
			}

			Personagem* speaker = findCharacter(conversation.name());

			// Testar se válido
			// O falante atual está na sala?
			if (speaker->getSalaAtual()->getNome() != it->getRoom())
				valid = false;

			// O falante atual preenche as condicoes?
			string info;
			if (valid)
				for (xml_node_iterator cit = conversation.begin(); cit != conversation.end(); ++cit) {
					info = cit->attribute("info").value();
					bool t1 = speaker->hasCondition(cit->name());
					string nao = cit->attribute("n").value();
					bool t2 = (nao == "n");
					if (info != "info" && t1 == t2) {
						valid = false;
						break;
					}
				}

			// Mensagem válida!
			if (valid) {
				// Send message
				speaker->sayLine("", conversation.attribute("line").value(), it->getParticipants(conversation.name()));
				// Lock every participant
				for (set<string>::iterator ait = it->getParticipants()->begin(); ait != it->getParticipants()->end(); ait++)
					findCharacter(*ait)->setInConversation(true);
				break;
			}
		}

		break;
	}
}


void Jogo::receiveArgs(vector<string> args) {
	player.receberArgs(args);
}


// LIDAR COM MAPA ------------------------------------------

vector<Personagem*> Jogo::getPeopleInRoom(Sala* room) {
	vector<Personagem*> retorno;
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getSalaAtual() == room)
			retorno.push_back(characters[i]);
	}

	return retorno;
}


// ACTIONS ----------------------------------
void Jogo::obtainObject(string name, Personagem* receiver) {
	vector<string> actionVec = FileManager::readFromFile("files/items/" + name + ".txt").getValues("actions");
	set<string> actionSet = set<string>();
	for (int i = 0; i < actionVec.size(); i++) {
		actionSet.insert(actionVec[i]);
	}

	receiver->addItem(name, actionSet);
	notify(_obter);
}


Sala* Jogo::moveRoom(Sala* origin, string destination) {
	// Movement
	origin = mapa.getSala(destination);
	return origin;
}


void Jogo::printText(string texto) {
	this->texto = texto;
	notify(_imprimir);
}


vector<Item> Jogo::getInventory() {
	return player.getInventario();
}


// HELPER FUNCTIONS ---------------------------------------------
#include <stdexcept>
Personagem* Jogo::findCharacter(string nome) {
	for (int i = 0; i < characters.size(); i++) {
		if (characters[i]->getNome() == nome)
			return characters[i];
	}

	throw invalid_argument("There's no character with that name :(");
}
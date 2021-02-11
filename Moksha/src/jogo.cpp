#include "jogo.h"
#include <iostream>

Jogo::Jogo() {
	jogador = Jogador();
	jogador.add(this, OBSERVER_OFFSET);
	personagens.push_back(&jogador);

	Baxter* baxter = new Baxter(&mapa);
	baxter->add(this, OBSERVER_OFFSET + 1);
	personagens.push_back(baxter);
	npcs.push_back(baxter);

	Hilda* hilda = new Hilda(&mapa);
	hilda->add(this, OBSERVER_OFFSET + 2);
	personagens.push_back(hilda);
	npcs.push_back(hilda);

	Santos* santos = new Santos(&mapa);
	santos->add(this, OBSERVER_OFFSET + 3);
	personagens.push_back(santos);
	npcs.push_back(santos);

	Jenna* jenna = new Jenna(&mapa);
	jenna->add(this, OBSERVER_OFFSET + 4);
	personagens.push_back(jenna);
	npcs.push_back(jenna);
}


void Jogo::setup() {
	initializeGame();
	loadGame();

	for (int i = 0; i < npcs.size(); i++)
		npcs[i]->setupPlanos();
}


void Jogo::update(int id) {
	// Objeto
	if (id < OBSERVER_OFFSET) {
		objetoOrdem(mapa.getObjeto(id));
	}

	// Personagem
	else {
		personagemOrdem(personagens[id - OBSERVER_OFFSET]);
	}
}


void Jogo::objetoOrdem(Objeto* objeto) {
	// Obter
	switch(objeto->getNotifyID()) {
	case objeto->obter:
		obterObjeto(objeto->getName(), findCharacter(objeto->getUser()));
		findCharacter(objeto->getUser())->getSalaAtual()->removeObjeto(*objeto);

		if (jogador.getNome() == objeto->getUser())
			advanceTime();
		break;
	}
}


void Jogo::personagemOrdem(Personagem* personagem) {
	int id = personagem->getNotifyID();
	string antigaSala;

	switch (id) {
	case personagem->imprimir:
		imprimirTexto(personagem->getNotifyText());
		break;

	case personagem->mover:
		// Pessoas dentro da sala veem pessoa saindo
		for (int i = 0; i < personagens.size(); i++)
			if (personagens[i] != personagem && personagens[i]->getSalaAtual() == personagem->getSalaAtual())
				personagens[i]->verPessoaMovendo(personagem, personagem->getNotifyText(), false);

		antigaSala = personagem->getSalaAtual()->getNome();
		personagem->setSalaAtual(moverSala(personagem->getSalaAtual(), personagem->getNotifyText()));
		personagem->verSala(getPessoasNaSala(personagem->getSalaAtual()));

		// Pessoas dentro da sala veem pessoa entrando
		for (int i = 0; i < personagens.size(); i++)
			if (personagens[i] != personagem && personagens[i]->getSalaAtual() == personagem->getSalaAtual())
				personagens[i]->verPessoaMovendo(personagem, antigaSala, true);
		break;

	case personagem->mencionar:
		for (int i = 0; i < personagens.size(); i++) {
			if (personagem->getNotifyTargets().count(personagens[i]->getNome()) && personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
				personagens[i]->executarReacao(personagem->getNotifyText(), "", personagem->getNome());
				break;
			}
		}
		break;

	case personagem->falar:
		for (int i = 0; i < personagens.size(); i++) {
			if (personagem->getNotifyTargets().count(personagens[i]->getNome()) && personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
				vector<string> args = splitString(personagem->getNotifyText(), '|');
				personagens[i]->executarReacao(args[0], args[1], personagem->getNome());
				break;
			}
		}
		break;

	case personagem->conversar:
		conversas.push_back(Conversa(personagem->getNotifyTargets(), personagem->getNotifyText(), personagem->getSalaAtual()->getNome()));
		advanceConversas();
		break;

	case personagem->descansar:
		break;

	case personagem->atacar:
		Personagem* vitima = findCharacter(personagem->getNotifyText());
		if (vitima->getSalaAtual() == personagem->getSalaAtual())
			vitima->serAtacado(personagem);
		break;
	}
	
	if (id != personagem->imprimir && personagem == &jogador)
		advanceTime();

}


void Jogo::advanceTime() {
	// Conversas
	advanceConversas();

	// Personagens
	for (int i = 0; i < npcs.size(); i++)
		npcs[i]->tomarAcao();
	
	// Jogo
	time++;
	saveGame();
}

void Jogo::advanceConversas() {
	// Iterar por cada conversa acontecendo
	for (vector<Conversa>::iterator it = conversas.begin(); it != conversas.end(); it++) {
		// Tentar até uma mensagem passar
		while (1) {
			// Se conversa acabou
			if (it->noFim()) {
				conversas.erase(it);
				break;
			}

			// Avancar
			bool valid = true;
			xml_node conversa = it->proximaFala();

			string n = conversa.name();
			if (n == "Narrador") {	// Edge case: narrador
				imprimirTexto(conversa.attribute("fala").value());
				break;
			}

			Personagem* falante = findCharacter(conversa.name());

			// Testar se válido
			// O falante atual está na sala?
			if (falante->getSalaAtual()->getNome() != it->getSala())
				valid = false;

			// O falante atual preenche as condicoes?
			if (valid)
				for (xml_node_iterator cit = conversa.begin(); cit != conversa.end(); ++cit) {
					bool t1 = falante->temCondicao(cit->name());
					string tt = cit->attribute("n").value();
					bool t2 = (tt == "n");
					if (cit->attribute("info").value() != "info" &&	t1 == t2) {
						valid = false;
						break;
					}
				}

			// Enviar mensagem
			if (valid) {
				falante->say("", conversa.attribute("fala").value(), it->getParticipantes(conversa.name()));
				break;
			}
		}

		if (!conversas.size())
			break;
	}
}


void Jogo::obterObjeto(string nome, Personagem* recebedor) {
	vector<string> acoesVec = FileManager::readFromFile("files/itens/" + nome + ".txt").getValues("acoes");
	set<string> acoesSet = set<string>();
	for (int i = 0; i < acoesVec.size(); i++) {
		acoesSet.insert(acoesVec[i]);
	}

	recebedor->addItem(nome, acoesSet);
	notify(obter);
}


void Jogo::receberArgs(vector<string> args) {
	jogador.receberArgs(args);
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
	vector<string> salaLista = FileManager::getFileList("files/salas");
	vector<Sala*> salas;

	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = FileManager::readFromFile(salaLista[i]);

		Sala* sala = new Sala(i, fileSala.getValue("nome"), fileSala.getValue("texto"),
			fileSala.getValues("adjacentes"), fileSala.getValues("objetos"));
		salas.push_back(sala);
	}

	mapa = Mapa(salas, this);

	// Gerar personagens
	xml_node load_package = doc.child("JogoData").child("Jogo").child("Personagens");
	int i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		string s = it->attribute("Sala").value();
		Personagem* personagem = findCharacter(it->name());
		personagem->setSalaAtual(mapa.getSala(s));
	}
}


bool Jogo::loadGame() {
	xml_document doc;
	if (!doc.load_file("files/save.xml"))
		return false;

	// Gerar jogo
	xml_node load_package = doc.child("JogoData").child("Jogo");
	time = load_package.attribute("Time").as_int();
	loop = load_package.attribute("Loop").as_int();

	// Gerar mapa
	load_package = doc.child("JogoData").child("Jogo").child("Mapa");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		Sala* estaSala = mapa.getSala(it->attribute("Nome").value());
		estaSala->limparObjetos();

		// Carregar objetos
		vector<string> objetoNomes;
		xml_node objetos = it->child("Objetos");

		for (xml_node_iterator ait = objetos.begin(); ait != objetos.end(); ++ait)
			objetoNomes.push_back(ait->name());

		estaSala->setObjetoNomes(objetoNomes);
		mapa.carregarSala(estaSala);
	}

	// Gerar personagens
	load_package = doc.child("JogoData").child("Jogo").child("Personagens");
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it) {
		string s = it->attribute("Sala").value();
		Personagem* personagem = findCharacter(it->name());
		personagem->setSalaAtual(mapa.getSala(s));

		// Carregar inventário
		xml_node inventario = it->child("Inventario");
		for (xml_node_iterator ait = inventario.begin(); ait != inventario.end(); ++ait) {
			obterObjeto(ait->name(), personagem);
		}
	}

	return true;
}


void Jogo::saveGame() {
	xml_document doc;
	if (!doc.load_file("files/save.xml"))
		doc.load_file("files/base.xml");
	xml_node node = doc.child("JogoData").child("Jogo");

	// Salvar jogo
	node.attribute("Time").set_value(to_string(time).c_str());
	node.attribute("Loop").set_value(to_string(loop).c_str());

	// Salvar mapa
	node = doc.child("JogoData").child("Jogo").child("Mapa");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Sala* thisSala = mapa.getSala(it->attribute("Nome").value());

		// Salvar objetos
		xml_node objetos = it->child("Objetos");
		objetos.remove_children();
		for (int i = 0; i < thisSala->getObjetoNomes().size(); i++) {
			objetos.append_child(thisSala->getObjetoNomes()[i].c_str());
		}
	}

	// Salvar personagens
	node = doc.child("JogoData").child("Jogo").child("Personagens");
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it) {
		Personagem* thisPersonagem = findCharacter(it->name());
		it->attribute("Sala").set_value( thisPersonagem->getSalaAtual()->getNome().c_str() );

		// Salvar inventario
		xml_node inventario = it->child("Inventario");
		inventario.remove_children();
		for (int i = 0; i < thisPersonagem->getInventario().size(); i++) {
			inventario.append_child(thisPersonagem->getInventario()[i].getNome().c_str());
		}
	}

	doc.save_file("files/save.xml");
}

// LIDAR COM MAPA ------------------------------------------

vector<Personagem*> Jogo::getPessoasNaSala(Sala* sala) {
	vector<Personagem*> retorno;
	for (int i = 0; i < personagens.size(); i++) {
		if (personagens[i]->getSalaAtual() == sala)
			retorno.push_back(personagens[i]);
	}

	return retorno;
}


// ACTIONS ----------------------------------
Sala* Jogo::moverSala(Sala* salaOrigem, string salaDestino) {
	// Movimento
	salaOrigem = mapa.getSala(salaDestino);
	return salaOrigem;
}


void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	notify(imprimir);
}


vector<Item> Jogo::getInventario() {
	return jogador.getInventario();
}


// HELPER FUNCTIONS ---------------------------------------------
#include <stdexcept>
Personagem* Jogo::findCharacter(string nome) {
	for (int i = 0; i < personagens.size(); i++) {
		if (personagens[i]->getNome() == nome)
			return personagens[i];
	}

	throw invalid_argument("There's no character with that name :(");
}
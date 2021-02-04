#include "jogo.h"
#include <iostream>

Jogo::Jogo() {
	jogador = Jogador();
	jogador.add(this, OBSERVER_OFFSET);
	personagens.push_back(&jogador);

	jenna = Jenna(&mapa);
	jenna.add(this, OBSERVER_OFFSET + 1);
	personagens.push_back(&jenna);
	npcs.push_back(&jenna);

	initializeGame();
	loadGame();
}


void Jogo::update(int id) {
	// Objeto
	if (id < OBSERVER_OFFSET) {
		objetoOrdem(getSalaAtual()->getObjeto(id));
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
		vector<string> acoesVec = FileManager::readFromFile("files/itens/" + objeto->getName() + ".txt").getValues("acoes");
		set<string> acoesSet = set<string>();
		for (int i = 0; i < acoesVec.size(); i++) {
			acoesSet.insert(acoesVec[i]);
		}

		findCharacter(objeto->getUser())->addItem(objeto->getName(), acoesSet);
		notify(obter);

		Sala* sala = jogador.getSalaAtual();
		sala->removeObjeto(*objeto);
		break;
	}
}


void Jogo::personagemOrdem(Personagem* personagem) {
	int id = personagem->getNotifyID();

	switch (id) {
	case personagem->imprimir:
		imprimirTexto(personagem->getNotifyText());
		break;

	case personagem->mover:
		personagem->setSalaAtual(moverSala(personagem->getSalaAtual(), personagem->getNotifyText()));
		personagem->verSala(getPessoasNaSala(personagem->getSalaAtual()));
		
		// Pessoas dentro da sala veem pessoa entrando
		for (int i = 0; i < personagens.size(); i++) {
			if (personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
				personagens[i]->verPessoaEntrando(personagem);
			}
		}

		break;

	case personagem->mencionar:
		for (int i = 0; i < personagens.size(); i++) {
			if (personagens[i]->getNome() == personagem->getNotifyTargets()[0]
					&& personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
				personagens[i]->executarReacao(personagem->getNotifyText(), "", personagem->getNome());
				break;
			}
		}
		break;

	case personagem->falar:
		for (int i = 0; i < personagens.size(); i++) {
			if (personagens[i]->getNome() == personagem->getNotifyTargets()[0]
					&& personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
				vector<string> args = splitString(personagem->getNotifyText(), '|');
				personagens[i]->executarReacao(args[0], args[1], personagem->getNome());
				break;
			}
		}
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
	// TODO: create a separate function for this
	if (jenna.getSalaAtual() != jogador.getSalaAtual())
		jenna.takeAction();
	
	time++;
	saveGame();
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

		Sala* sala = new Sala(fileSala.getValue("nome"), fileSala.getValue("texto"),
			fileSala.getValues("adjacentes"), fileSala.getValues("objetos"));
		salas.push_back(sala);
	}

	mapa = Mapa(salas, this);

	// Gerar personagens
	xml_node load_package = doc.child("JogoData").child("Jogo").child("Personagens");
	int i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		string s = it->attribute("Sala").value();
		personagens[i]->setSalaAtual(mapa.getSala(s));
		if (i > 0) {
			s = it->attribute("Alvo").value();
			npcs[i - 1]->setSalaAlvo(mapa.getSala(s));
		}
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
	int i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		mapa.getSala(i)->limparObjetos();

		// Carregar objetos
		vector<string> objetoNomes;
		xml_node objetos = it->child("Objetos");
		for (xml_node_iterator ait = objetos.begin(); ait != objetos.end(); ++ait)
			objetoNomes.push_back(ait->name());

		mapa.getSala(i)->setObjetoNomes(objetoNomes);
		mapa.carregarSala(mapa.getSala(i));
	}

	// Gerar personagens
	load_package = doc.child("JogoData").child("Jogo").child("Personagens");
	i = 0;
	for (xml_node_iterator it = load_package.begin(); it != load_package.end(); ++it, i++) {
		string s = it->attribute("Sala").value();
		personagens[i]->setSalaAtual(mapa.getSala(s));
		if (i > 0) {
			s = it->attribute("Alvo").value();
			npcs[i - 1]->setSalaAlvo(mapa.getSala(s));
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
		it->attribute("Sala").set_value( thisPersonagem->getSalaAtual()->getName().c_str() );

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
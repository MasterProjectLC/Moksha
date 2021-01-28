#include "jogo.h"

Jogo::Jogo() {
	gerarMapa();

	jogador = Jogador();
	jogador.setSalaAtual(mapa.getSala("Corredor"));
	jogador.add(this, OBSERVER_OFFSET);
	personagens.push_back(&jogador);

	jenna = Jenna(&mapa);
	jenna.setSalaAtual(mapa.getSala("Banheiro"));
	jenna.setSalaAlvo(mapa.getSala("Corredor"));
	jenna.add(this, OBSERVER_OFFSET+1);
	personagens.push_back(&jenna);

	mapa.carregarSala(jogador.getSalaAtual());
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
}


void Jogo::receberArgs(vector<string> args) {
	jogador.receberArgs(args);
}

// LIDAR COM MAPA ------------------------------------------

void Jogo::gerarMapa() {
	vector<string> salaLista = FileManager::getFileList("files/salas");

	// Gerar salas e colocá-los no Mapa
	vector<Sala*> salas;
	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = FileManager::readFromFile(salaLista[i]);
		salas.push_back(new Sala(fileSala.getValue("nome"), fileSala.getValue("texto"),
							fileSala.getValues("adjacentes"), fileSala.getValues("objetos")));
	}

	mapa = Mapa(salas, this);
}


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
	mapa.carregarSala(salaOrigem);
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
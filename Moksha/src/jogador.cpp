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

void Jogador::mention(string obj, set<string> receivers) {
	if (inventario.temConceito(obj) || inventario.temItem(obj)) {
		Personagem::mention(obj, receivers);
	}
	else {
		printText(erroSemItem);
	}
}


void Jogador::mencionar(string topic, string person) {
	if (mindTheory.count(person) && mindTheory.at(person).find(topic) != mindTheory.at(person).end()) {
		printText(erroMente);
		return;
	}

	if (!inventario.temConceito(topic) && !inventario.temItem(topic)) {
		printText(erroSemItem);
		return;
	}

	mention(topic, set<string>({ person }));
}


void Jogador::mover(string location) {
	if (getSalaAtual()->isSalaAnexa(location))
		move(location);
	else
		printText(erroSemSala);
}


void Jogador::interagir(string acao, string objeto) {
	// Objetos
	if (getSalaAtual()->possuiObjeto(objeto)) {
		Objeto* objetoAqui = getSalaAtual()->getObjeto(objeto);

		// Imprimir resposta
		vector<string> responses = objetoAqui->getResponses(acao);
		if (responses.size() > 0) { printText(responses[0]); }
		else { printText(erroSemAcao); }

		// Tomar ação
		objetoAqui->takeAction(acao, nome);
	}

	// Objeto não existe
	else
		printText(erroSemObjeto);
}

void Jogador::receberArgs(vector<string> args) {
	if (!isAcaoValida(args.at(0)))
		return;

	if (args.size() == 3) {
		// Mencionar
		if (args.at(0) == "mention") {
			mencionar(args.at(1), args.at(2));
			return;
		}
	}
	
	if (args.size() >= 2) {
		string secondArg = concatStrings(args, 1);

		// Mover para outra sala
		if (args.at(0) == "move") {
			mover(secondArg);
		}
		else if (args.at(0) == "attack") {
			attack(secondArg);
		}

		else {
			// Objetos
			interagir(args.at(0), secondArg);
		}
	}

	if (args.at(0) == "rest" || args.at(0) == "wait") {
		rest();
	}
}


// REACOES -----------------------------------------------------------------------

void Jogador::executarReacao(string topico, string frase, string remetente) {
	printText(remetente + ": " + frase);
	addToMind(topico, remetente);
}


void Jogador::verSala(vector<Personagem*> pessoasNaSala) {
	// Salas anexas
	printText("Current room: " + getSalaAtual()->getNome() + "\n" + getSalaAtual()->getTextoInicial() + "\nAdjacent rooms:");
	for (int i = 0; i < getSalaAtual()->getSalaAnexaCount(); i++)
		printText(getSalaAtual()->getSalaAnexaNome(i));

	// Objetos na sala
	printText("Objects in the room: ");
	vector<Objeto> objetos = getSalaAtual()->getObjetos();
	if (objetos.size() == 0)
		printText("There's no object here.");
	else {
		for (int i = 0; i < objetos.size(); i++) {
			printText(objetos[i].getName());
		}
	}

	// Pessoas na sala
	for (int i = 0; i < pessoasNaSala.size(); i++) {
		if (pessoasNaSala[i]->getNome() != nome)
			if (!pessoasNaSala[i]->isInconsciente())
				printText(pessoasNaSala[i]->getNome() + " is in the room.");
			else
				printText("Oh, " + pessoasNaSala[i]->getNome() + " is unconscious here!");
	}
}


void Jogador::verPessoaMovendo(Personagem* pessoa, string outraSala, bool entrando) {
	if (entrando)
		printText(pessoa->getNome() + " entered the room, coming from the " + outraSala);
	else
		printText(pessoa->getNome() + " left the room, going to the " + outraSala);
}

// HELPER FUNCTIONS ----------------------------------------------------

bool Jogador::temCondicao(string info) {
	return inventario.temConceito(info);
}


bool Jogador::isAcaoValida(string acao) {
	for (int i = 0; i < acoesBasicas.size(); i++)
		if (acoesBasicas[i].compare(acao) == 0)
			return true;

	for (int i = 0; i < getInventario().size(); i++)
		if (getInventario()[i].isAcaoValida(acao))
			return true;

	return false;
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
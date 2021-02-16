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
		move(location);
	else
		printText(erroSemSala);
}


void Jogador::interact(string acao, string objeto) {
	// Objetos
	if (getSalaAtual()->possuiObjeto(objeto)) {
		Objeto* objetoAqui = getSalaAtual()->getObjeto(objeto);

		// Imprimir resposta
		vector<string> responses = objetoAqui->getResponses(acao);
		if (responses.size() > 0) { printText(responses[0]); }
		else { printText(erroSemAcao); }

		// Tomar ação
		interact(acao, objeto);
	}

	// Objeto não existe
	else
		printText(erroSemObjeto);
}


void Jogador::receberArgs(vector<string> args) {
	int action;
	for (action = 2; action < stringEnum.size(); action++)
		if (stringEnum[action] == args.at(0))
			break;
	args.erase(args.begin());

	takeAction(action, args);
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
			if (!pessoasNaSala[i]->isUnconscious())
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
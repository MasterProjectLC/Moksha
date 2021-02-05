#include "jogador.h"

Jogador::Jogador() : Personagem(M, 2, 2) {
	nome = "Colt";

	FileDict fileErros = FileManager::readFromFile("files/erros.txt");
	erroSemObjeto = fileErros.getValues("sem objeto")[0];
	erroSemAcao = fileErros.getValues("sem acao")[0];
	erroSemSala = fileErros.getValues("sem sala")[0];
	erroSemItem = fileErros.getValues("sem item")[0];
	erroMente = fileErros.getValues("teoria da mente")[0];
};


// ACOES -------------------------------------------------------------------------------------

void Jogador::mencionar(string topic, string person) {
	if (mindTheory.count(person) && mindTheory.at(person).find(topic) != mindTheory.at(person).end()) {
		printText(erroMente);
		return;
	}

	if (!inventario.temConceito(topic) && !inventario.temItem(topic)) {
		printText(erroSemItem);
		return;
	}

	mention(topic, vector<string>(1, person));
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
		if (args.at(0) == "mencionar") {
			mencionar(args.at(1), args.at(2));
			return;
		}
	}
	
	if (args.size() >= 2) {
		string secondArg = concatStrings(args, 1);

		// Mover para outra sala
		if (args.at(0) == "mover") {
			mover(secondArg);
		}
		else if (args.at(0) == "atacar") {
			attack(secondArg);
		}

		else {
			// Objetos
			interagir(args.at(0), secondArg);
		}
	}
}


// REACOES -----------------------------------------------------------------------

void Jogador::executarReacao(string topico, string frase, string remetente) {
	printText(remetente + ": " + frase);
	addToMind(topico, remetente);
}


void Jogador::verSala(vector<Personagem*> pessoasNaSala) {
	// Salas anexas
	printText("Sala atual: " + getSalaAtual()->getNome() + "\n" + getSalaAtual()->getTextoInicial() + "\nSalas anexas:");
	for (int i = 0; i < getSalaAtual()->getSalaAnexaCount(); i++)
		printText(getSalaAtual()->getSalaAnexaNome(i));

	// Objetos na sala
	printText("Objetos na sala: ");
	vector<Objeto> objetos = getSalaAtual()->getObjetos();
	for (int i = 0; i < objetos.size(); i++) {
		printText(objetos[i].getName());
	}

	// Pessoas na sala
	for (int i = 0; i < pessoasNaSala.size(); i++) {
		if (pessoasNaSala[i]->getNome() != nome)
			if (!pessoasNaSala[i]->isInconsciente())
				printText(pessoasNaSala[i]->getNome() + " esta nesta sala.");
			else
				printText("Na sala, " + pessoasNaSala[i]->getNome() + " esta inconsciente!");
	}
}


void Jogador::verPessoaEntrando(string nomePessoa) {
	printText(nomePessoa + " entrou na sala.");
}

// HELPER FUNCTIONS ----------------------------------------------------

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
#include "jogador.h"

Jogador::Jogador() {
	nome = "Colt";

	FileDict fileErros = FileManager::readFromFile("files/erros.txt");
	erroSemObjeto = fileErros.getValues("sem objeto")[0];
	erroSemAcao = fileErros.getValues("sem acao")[0];
	erroSemSala = fileErros.getValues("sem sala")[0];
};


void Jogador::receberArgs(vector<string> args) {
	if (!isAcaoValida(args.at(0)))
		return;

	if (args.size() == 3) {
		// Mencionar
		if (args.at(0) == "mencionar" && (!mindTheory.count(args.at(2)) || 
						mindTheory.at(args.at(2)).find(args.at(1)) == mindTheory.at(args.at(2)).end())
		   ) {
			mention(args.at(1), vector<string>(1, args.at(2)));
			return;
		}

	} 
	
	if (args.size() >= 2) {
		string secondArg = concatStrings(args, 1);

		// Mover para outra sala
		if (args.at(0) == "mover") {
			if (getSalaAtual()->isSalaAnexa(secondArg))
				move(secondArg);
			else
				printText(erroSemSala);
		}

		// Objetos
		else {
			if (getSalaAtual()->possuiObjeto(secondArg)) {
				Objeto objetoAqui = getSalaAtual()->getObjeto(secondArg);
				objetoAqui.takeAction(args.at(0));

				// Imprimir resposta
				vector<string> responses = objetoAqui.getResponses(args.at(0));
				if (responses.size() > 0) { printText(responses[0]); }
				else { printText(erroSemAcao); }
			}

			// Objeto não existe
			else
				printText(erroSemObjeto);
		}
	}
}


void Jogador::executarReacao(string topico, string frase, string remetente) {
	printText(remetente + ": " + frase);
	addToMind(topico, remetente);
}


void Jogador::verSala(vector<string> pessoasNaSala) {
	// Salas anexas
	printText("Sala atual: " + getSalaAtual()->getName() + "\n" + getSalaAtual()->getTextoInicial() + "\nSalas anexas:");
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
		if (pessoasNaSala[i] != nome)
			printText(pessoasNaSala[i] + " esta nesta sala.");
	}
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


void Jogador::verPessoaEntrando(string nomePessoa) {
	printText(nomePessoa + " entrou na sala.");
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
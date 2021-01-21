#include "jogador.h"

Jogador::Jogador() {
	FileDict fileErros = FileManager::readFromFile("files/erros.txt");
	erroSemObjeto = fileErros.getValues("sem objeto")[0];
	erroSemAcao = fileErros.getValues("sem acao")[0];
	erroSemSala = fileErros.getValues("sem sala")[0];
};

void Jogador::receberArgs(vector<string> args) {
	if (args.size() >= 2 && isAcaoValida(args.at(0))) {
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
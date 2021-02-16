#include "item.h"

Item::Item(string nome, string descricao, set<string> acoes) : Conceito(nome, descricao) {
	this->acoes = acoes;

	for (set<string>::iterator it = acoes.begin(); it != acoes.end(); it++) {
		string s = *it;
		if (s == "atacar")
			actions.insert(atacar);
	}
}

bool Item::isAcaoValida(string acao) {
	return acoes.count(acao) > 0;
}

bool Item::isActionValid(int action) {
	return actions.count(action) > 0;
}
#include "item.h"

Item::Item(string nome) : Conceito(nome) {
	this->acoes = vector<string>();
}


Item::Item(string nome, vector<string> acoes) : Conceito(nome) {
	this->acoes = acoes;
}

bool Item::isAcaoValida(string acao) {
	for (int i = 0; i < acoes.size(); i++) {
		if (acoes[i].compare(acao) == 0) {
			return true;
		}
	}
	return false;
}
#include "item.h"

Item::Item(string nome) : Conceito(nome) {
	this->acoes = set<string>();
}


Item::Item(string nome, set<string> acoes) : Conceito(nome) {
	this->acoes = acoes;
}

Item::Item(string nome, string descricao, set<string> acoes) : Conceito(nome, descricao) {
	this->acoes = acoes;
}

bool Item::isAcaoValida(string acao) {
	return acoes.count(acao) > 0;
}
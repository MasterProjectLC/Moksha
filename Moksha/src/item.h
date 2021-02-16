#pragma once
#include <vector>
#include <set>
#include "conceito.h"
#include "actionEnum.h"

using namespace std;

class Item : public Conceito {
private:
	set<string> acoes;
	set<int> actions;

public:
	Item(string nome) : Item(nome, "", set<string>()) {};
	Item(string nome, set<string> acoes) : Item(nome, "", acoes) {};
	Item(string nome, string descricao, set<string> acoes);

	bool isAcaoValida(string acao);
	bool isActionValid(int action);
};

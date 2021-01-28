#pragma once
#include <vector>
#include <set>
#include "conceito.h"

using namespace std;

class Item : public Conceito {
private:
	set<string> acoes;

public:
	Item(string nome);
	Item(string nome, set<string> acoes);
	Item(string nome, string descricao, set<string> acoes);

	bool isAcaoValida(string acao);
};

#pragma once
#include <vector>
#include "conceito.h"

using namespace std;

class Item : public Conceito {
private:
	vector<string> acoes;

public:
	Item(string nome);
	Item(string nome, vector<string> acoes);

	bool isAcaoValida(string acao);
};

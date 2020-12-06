#pragma once
#include "inventario.h"
#include <vector>
#include <string>
#include "IObservable.h"

using namespace std;

class Jogo : public IObservable {
private:
	Inventario inventario;

public:
	vector<Conceito> getInventario();
	void receberArgs(vector<string> args);
};
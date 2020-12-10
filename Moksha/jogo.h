#pragma once
#include "inventario.h"
#include <vector>
#include <string>
#include "IObservable.h"

using namespace std;

class Jogo : public IObservable {
private:
	Inventario inventario;
	string concatenarArgs(vector<string> args, int inicio);
public:
	vector<Conceito> getInventario();
	void receberArgs(vector<string> args);
};
#pragma once
#include "conceito.h"
#include <vector>

using namespace std;

class Inventario {
private:
	vector<Conceito> inventario;

public:
	vector<Conceito> getInventario();
	void addConceito(string nome);
};

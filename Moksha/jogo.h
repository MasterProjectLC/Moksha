#pragma once
#include "inventario.h"
#include "IObservable.h"
#include "sala.h"
#include "fileManager.h"

using namespace std;

class Jogo : public IObservable {
private:
	FileManager fileManager;

	Sala salaAtual;
	vector<Sala> salas;

	Inventario inventario;
	string concatenarArgs(vector<string> args, int inicio);

	string texto;

public:
	Jogo();

	enum { obter, imprimir };

	Sala getSalaAtual() { return salaAtual; }
	void receberArgs(vector<string> args);

	vector<Conceito> getInventario();

	void imprimirTexto(string texto);
	string getTexto() { return texto; }
};
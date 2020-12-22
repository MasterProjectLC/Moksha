#pragma once
#include "personagem.h"
#include "IObservable.h"
#include "IObserver.h"
#include "sala.h"
#include "fileManager.h"

using namespace std;

class Jogo : public IObservable, public IObserver {
private:
	FileManager fileManager;
	string texto;

	Sala salaAtual;
	vector<Sala> salas;
	vector<Objeto> objetos;

	Personagem jogador;

	string erroSemObjeto;
	string erroSemAcao;

	void generateSalas();
	void carregarSala(Sala *sala);
	void moverSala(Sala* salaOrigem, string salaDestino);

	void addItem(string item);

public:
	Jogo();

	void update(int id) override;

	enum { obter, imprimir };

	Sala getSalaAtual() { return salaAtual; }
	void receberArgs(vector<string> args);

	vector<Item> getInventario();

	void imprimirTexto(string texto);
	string getTexto() { return texto; }
};
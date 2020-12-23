#pragma once
#include "personagem.h"
#include "IObservable.h"
#include "IObserver.h"
#include "mapa.h"
#include "fileManager.h"

using namespace std;

class Jogo : public IObservable, public IObserver {
private:
	FileManager fileManager;
	string texto;

	//vector<Objeto> objetos;
	Personagem jogador;

	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;

	void addItem(string item);

	void gerarMapa();
	void carregarSala(Sala *sala);
	Sala moverSala(Sala salaOrigem, string salaDestino);
	Mapa mapa;

	class No;

public:
	Jogo();

	void update(int id) override;

	enum { obter, imprimir };

	Sala* getSalaAtual() { return jogador.getSalaAtual(); }
	void receberArgs(vector<string> args);

	vector<Item> getInventario();

	void imprimirTexto(string texto);
	string getTexto() { return texto; }
};
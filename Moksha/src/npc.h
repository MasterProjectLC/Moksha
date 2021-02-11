#pragma once
#include "personagem.h"
#include "mapa.h"
#include "fileManager.h"
#include "dictionary.h"
#include "../libs/GPGOAP/goap.h"
#include "../libs/GPGOAP/astar.h"  // ASTER GASTER WOWOWOWOW

using namespace std;

class NPC : public Personagem {
protected:
	int MAX_ALVOS = 6;
	vector<string> alvos;
	queue<Sala*> caminho;
	Mapa* mapa;
	string conversaAlvo;

	actionplanner_t ap;
	worldstate_t states[16];
	worldstate_t current, goal;
	const char* plan[16]; // The planner will return the action plan in this array.
	int plansz = 16; // Size of our return buffers
	int planCost = 0;
	int currentStep = 0;
	Dictionary<vector<string>> dict;

	queue<Sala*> descobrirCaminho(Sala* salaInicial, Sala* salaAlvo);
	queue<Sala*> descobrirCaminho(Sala* salaAlvo);
	queue<Sala*> procurar();
	queue<Sala*> procurar(Sala* salaPista);

	int tamanhoCaminho(Sala* inicio, Sala* alvo) {
		return mapa->optimalPath(inicio, alvo).size();
	};

	void seguirCaminho();

	virtual void setupMundoAdicional() {}
	virtual void setupObjetivosAdicional() {}
	virtual void setupAcoesAdicional() {}
	virtual void updatePlanos() {}
	virtual void avancarPlanos() {}
	virtual void unlockPlanos() {}

	int alvoIndex(string a);

public:
	explicit NPC(Mapa* m, string nome, int genero, int forca, int destreza);

	bool temCondicao(string info) override;

	void executarReacao(string topico, string frase, string remetente) override;
	void verSala(vector<Personagem*> pessoasNaSala) override;
	void verPessoaMovendo(Personagem * pessoa, string outraSala, bool entrando) override;
	void setSalaAlvo(Sala* nova) { descobrirCaminho(nova); }

	void iniciarConversa();

	void setupPlanos();
};
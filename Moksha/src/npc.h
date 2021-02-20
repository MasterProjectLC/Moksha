#pragma once
#include <queue>
#include "goal.h"
#include "priorityVector.h"
#include "personagem.h"
#include "mapa.h"
#include "fileManager.h"
#include "dictionary.h"
#include "../libs/GPGOAP/goap.h"
#include "../libs/GPGOAP/astar.h"  // ASTER GASTER WOWOWOWOW

using namespace std;

class NPC : public Personagem {
protected:
	const vector<string> nomes = { "Elliot", "Baxter", "Willow", "Hilda", "Santos",
								"Magnus", "Tom", "Jenna", "Renard", "Liz",
								"George", "Damian", "Amelie" };

	int MAX_ALVOS = 6;
	vector<string> alvos;
	queue<Sala*> path;
	Mapa* mapa;
	string conversaAlvo;

	int currentAction;
	vector<string> actionArgs;

	actionplanner_t ap;
	worldstate_t states[16];
	worldstate_t world;
	Goal currentGoal;
	PriorityVector<Goal> goalList;
	const char* plan[16]; // The planner will return the action plan in this array.
	int plansz = 16; // Size of our return buffers
	int planCost = 0;
	int currentStep = 0;
	Dictionary<vector<string>> dict;

	queue<Sala*> findPath(Sala* salaInicial, Sala* salaAlvo);
	queue<Sala*> findPath(Sala* salaAlvo);
	queue<Sala*> search();
	queue<Sala*> search(Sala* salaPista);

	int tamanhoCaminho(Sala* inicio, Sala* alvo) {
		return mapa->optimalPath(inicio, alvo).size();
	};

	string nextRoomInPath();

	virtual int decidirAcaoAdicional(string acao) { return descansar; }
	virtual void setupMundoAdicional() {}
	virtual void setupObjetivosAdicional() {}
	virtual void setupAcoesAdicional() {}
	void updateWorld();
	virtual void updateWorldExtra() {}
	void advancePlans();
	virtual void advancePlansExtra(string currentProcess) {}
	void changePlans() { changePlans(false); };
	void changePlans(bool justUpdated);

	int alvoIndex(string a);

public:
	explicit NPC(Mapa* m, string nome, int genero, int forca, int destreza);

	bool hasCondition(string info) override;
	void takeAction() override { Personagem::takeAction(currentAction, actionArgs); updateWorld(); }

	int decideAction();
	void executeReaction(string topico, string frase, string remetente) override;
	void checkRoom(vector<Personagem*> pessoasNaSala) override;
	void seeCharMoving(Personagem * pessoa, string outraSala, bool entrando) override;
	void setSalaAlvo(Sala* nova) { findPath(nova); }

	void setupPlans();

	int getAction() { return currentAction; }
};
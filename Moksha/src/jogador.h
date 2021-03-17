#pragma once
#include <map>
#include <set>
#include "npc.h"
#include "stringLib.h"
#include "fileManager.h"

using namespace std;

class Jogador : public Personagem {
private:
	string noObjectError;
	string noActionError;
	string noRoomError;
	string noItemError;
	string mindError;

	map<string, set<string>> mindTheory;

	void mention(string obj, string receiver) override;
	void move(string location) override;
	void interact(string acao, string objeto) override;

	void addToMind(string topic, string character);

public:
	Jogador();
	void receberArgs(vector<string> args);

	bool hasCondition(string info) override;

	void seeCharMoving(Personagem* pessoa, string outraSala, bool entrando) override;
	void executeReaction(string topico, string frase, string remetente, bool shouldRespond) override;
	void receiveCheck(Personagem* checkTarget) override;
	void checkRoom(vector<Personagem*> pessoasNaSala) override;
	void updateRoom(vector<Personagem*> pessoasNaSala);

	bool beAttacked(Personagem* atacante) override {
		printText(atacante->getName() + " attacked you!");
		return Personagem::beAttacked(atacante);
	};
};
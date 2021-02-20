#pragma once
#include "../npc.h"

using namespace std;

class Jenna: public NPC {
private:

public:
	Jenna() : NPC{ NULL, "Jenna", F, 1, 1 } {}
	explicit Jenna(Mapa* m);
	int decidirAcaoAdicional(string acao) override;
	void advancePlansExtra(string currentProcess) override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updateWorldExtra() override;
	void setupAcoesAdicional() override;
};
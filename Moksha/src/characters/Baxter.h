#pragma once
#include "../npc.h"

using namespace std;

class Baxter : public NPC {
private:

public:
	Baxter() : NPC{ NULL, "Baxter", "", M, 0, 0 } {}
	explicit Baxter(Mapa* m);
	int decidirAcaoAdicional(string acao) override;
	void advancePlansExtra(string currentProcess) override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updateWorldExtra() override;
	void setupAcoesAdicional() override;
};
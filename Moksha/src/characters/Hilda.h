#pragma once
#include "../npc.h"

using namespace std;

class Hilda : public NPC {
private:

public:
	Hilda() : NPC{ NULL, "Hilda", "", F, 2, 1 } {}
	explicit Hilda(Mapa* m);
	int decidirAcaoAdicional(string acao) override;
	void advancePlansExtra(string currentProcess) override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updateWorldExtra() override;
	void setupAcoesAdicional() override;
};
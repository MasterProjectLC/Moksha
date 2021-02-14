#pragma once
#include "../npc.h"

using namespace std;

class Jenna: public NPC {
private:

public:
	Jenna() : NPC{ NULL, "Jenna", F, 1, 1 } {}
	explicit Jenna(Mapa* m);
	void tomarAcaoParticular(string acao) override;
	void avancarPlanosAdicional() override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updatePlanosAdicional() override;
	void setupAcoesAdicional() override;
};
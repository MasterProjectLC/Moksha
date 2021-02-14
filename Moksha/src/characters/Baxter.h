#pragma once
#include "../npc.h"

using namespace std;

class Baxter : public NPC {
private:

public:
	Baxter() : NPC{ NULL, "Baxter", M, 0, 0 } {}
	explicit Baxter(Mapa* m);
	void tomarAcaoParticular(string acao) override;
	void avancarPlanosAdicional() override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updatePlanosAdicional() override;
	void setupAcoesAdicional() override;
};
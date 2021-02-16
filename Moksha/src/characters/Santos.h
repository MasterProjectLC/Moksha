#pragma once
#include "../npc.h"

using namespace std;

class Santos : public NPC {
private:

public:
	Santos() : NPC{ NULL, "Santos", M, 3, 1 } {}
	explicit Santos(Mapa* m);
	int decidirAcaoAdicional(string acao) override;
	void avancarPlanosAdicional() override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updatePlanosAdicional() override;
	void setupAcoesAdicional() override;
};
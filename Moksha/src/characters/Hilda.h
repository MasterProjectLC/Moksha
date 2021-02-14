#pragma once
#include "../npc.h"

using namespace std;

class Hilda : public NPC {
private:

public:
	Hilda() : NPC{ NULL, "Hilda", M, 2, 1 } {}
	explicit Hilda(Mapa* m);
	void tomarAcaoParticular(string acao) override;
	void avancarPlanosAdicional() override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void updatePlanosAdicional() override;
	void setupAcoesAdicional() override;
};
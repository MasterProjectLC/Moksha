#pragma once
#include "../npc.h"

using namespace std;

class Santos : public NPC {
private:

public:
	Santos() : NPC{ NULL, "Santos", M, 3, 1 } {}
	explicit Santos(Mapa* m);
	void tomarAcao() override;
	void setupMundoAdicional() override;
	void setupObjetivosAdicional() override;
	void setupAcoesAdicional() override;
	void updatePlanos() override;
	void avancarPlanos() override;
};
#include "Santos.h"

Santos::Santos(Mapa* m) : NPC{ m, "Santos", F, 3, 2 } {
	alvos.push_back("Hilda");
	alvos.push_back("Baxter");
}

void Santos::setupAcoesAdicional() {


	goap_set_pre(&ap, "talk_no_gun_Hilda", "with_Hilda", true);
	goap_set_pst(&ap, "talk_no_gun_Hilda", "talk_no_gun", true);

	goap_set_pre(&ap, "talk_no_gun_Baxter", "with_Baxter", true);
	goap_set_pst(&ap, "talk_no_gun_Baxter", "talk_no_gun", true);
}


void Santos::setupMundoAdicional() {
	goap_worldstate_set(&ap, &current, "with_Baxter", true);
	goap_worldstate_set(&ap, &current, "with_Hilda", true);
	goap_worldstate_set(&ap, &current, "talk_no_gun", false);
}


void Santos::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &goal, "talk_no_gun", true);
}


void Santos::updatePlanosAdicional() {
	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Santos::tomarAcaoParticular(string acao) {
	if (acao == "talk_no_gun_Hilda" || acao == "talk_no_gun_Baxter") {
		talk("santos_gun");
		goap_worldstate_set(&ap, &current, "talk_no_gun", true);

		if (caminho.size() == 0)
			avancarPlanos();
	}
}


void Santos::avancarPlanosAdicional() {
	if (plan[currentStep] == "move_kitchen") {
		caminho = descobrirCaminho(mapa->getSala("Kitchen"));
	}

}
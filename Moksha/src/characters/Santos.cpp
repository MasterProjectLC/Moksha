#include "Santos.h"

Santos::Santos(Mapa* m) : NPC{ m, "Santos", F, 3, 2 } {
	alvos.push_back("Hilda");
	alvos.push_back("Baxter");
}

void Santos::setupAcoesAdicional() {


	goap_set_pre(&ap, "conversar_sem_pistola_Hilda", "com_Hilda", true);
	goap_set_pst(&ap, "conversar_sem_pistola_Hilda", "conversa_sem_pistola", true);

	goap_set_pre(&ap, "conversar_sem_pistola_Baxter", "com_Baxter", true);
	goap_set_pst(&ap, "conversar_sem_pistola_Baxter", "conversa_sem_pistola", true);
}


void Santos::setupMundoAdicional() {
	goap_worldstate_set(&ap, &current, "com_Baxter", true);
	goap_worldstate_set(&ap, &current, "com_Hilda", true);
	goap_worldstate_set(&ap, &current, "conversa_sem_pistola", false);
}


void Santos::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &goal, "conversa_sem_pistola", true);
}


void Santos::updatePlanos() {
	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Santos::tomarAcao() {
	string acao = plan[currentStep];

	if (acao == "conversar_sem_pistola_Hilda" || acao == "conversar_sem_pistola_Baxter") {
		talk("pistola_santos", { "Santos", "Hilda", "Baxter", "Ned" });
		goap_worldstate_set(&ap, &current, "conversa_sem_pistola", true);

		if (caminho.size() == 0)
			avancarPlanos();
	}
}


void Santos::avancarPlanos() {
	currentStep++;

	if (currentStep >= plansz)
		updatePlanos();

	if (plan[currentStep] == "mover_cozinha") {
		caminho = descobrirCaminho(mapa->getSala("Cozinha"));
	}

}
#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna", F, 1, 1} {
	alvos.push_back("Elliot");
}

void Jenna::setupAcoesAdicional() {
	goap_set_pst(&ap, "move_kitchen", "in_kitchen", true);
	goap_set_cost(&ap, "move_kitchen", tamanhoCaminho(salaAtual, mapa->getSala("Kitchen")));

	goap_set_pre(&ap, "take_knife", "in_kitchen", true);
	goap_set_pst(&ap, "take_knife", "armed", true);

	goap_set_pre(&ap, "search_target", "armed", true);
	goap_set_pst(&ap, "search_target", "with_Elliot", true);
	goap_set_cost(&ap, "search_target", 2);

	goap_set_pre(&ap, "kill", "with_Elliot", true);
	goap_set_pre(&ap, "kill", "armed", true);
	goap_set_pst(&ap, "kill", "Elliot_alive", false);
}


void Jenna::setupMundoAdicional() {
	goap_worldstate_set(&ap, &current, "in_kitchen", salaAtual->getNome() == "Kitchen");
}


void Jenna::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &goal, "Elliot_alive", false);
}


void Jenna::updatePlanosAdicional() {
	// describe current world state.
	goap_worldstate_set(&ap, &current, "in_kitchen", salaAtual->getNome() == "Kitchen");
	goap_worldstate_set(&ap, &current, "armed", inventario.temItem("Knife"));

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Jenna::tomarAcaoParticular(string acao) {
	if (acao == "move_kitchen") {
		seguirCaminho();

		if (caminho.size() == 0)
			avancarPlanos();
	}

	else if (acao == "search_target") {
		seguirCaminho();

		if (caminho.size() == 0)
			avancarPlanos();
	}

	else if (acao == "take_knife") {
		salaAtual->getObjeto("Knife")->takeAction("obtain", nome);
		if (inventario.temItem("Knife"))
			avancarPlanos();
	}

	else if (acao == "kill") {
		attack(alvos[0]);
		goap_worldstate_set(&ap, &current, (alvos[0] + "_alive").c_str(), false);
		avancarPlanos();
	}
}


void Jenna::avancarPlanosAdicional() {
	if (plan[currentStep] == "move_kitchen") {
		caminho = descobrirCaminho(mapa->getSala("Kitchen"));
	}

	else if (plan[currentStep] == "search_target") {
		if (ultimoAvistamento.hasKey(alvos[0]))
			caminho = procurar(mapa->getSala(ultimoAvistamento.getValues(alvos[0])));
		else
			caminho = procurar();

	}
}
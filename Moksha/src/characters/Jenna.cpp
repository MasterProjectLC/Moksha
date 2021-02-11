#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna", F, 1, 1} {
	alvos.push_back("Ned");
}

void Jenna::setupAcoesAdicional() {
	goap_set_pst(&ap, "mover_cozinha", "na_cozinha", true);
	goap_set_cost(&ap, "mover_cozinha", tamanhoCaminho(salaAtual, mapa->getSala("Cozinha")));

	goap_set_pre(&ap, "pegar_faca", "na_cozinha", true);
	goap_set_pst(&ap, "pegar_faca", "armado", true);

	goap_set_pre(&ap, "procurar_alvo", "armado", true);
	goap_set_pst(&ap, "procurar_alvo", "com_Ned", true);
	goap_set_cost(&ap, "procurar_alvo", 2);

	goap_set_pre(&ap, "matar", "com_Ned", true);
	goap_set_pre(&ap, "matar", "armado", true);
	goap_set_pst(&ap, "matar", "Ned_vivo", false);
}


void Jenna::setupMundoAdicional() {
	goap_worldstate_set(&ap, &current, "na_cozinha", salaAtual->getNome() == "Cozinha");
}


void Jenna::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &goal, "Ned_vivo", false);
}


void Jenna::updatePlanos() {
	// describe current world state.
	goap_worldstate_set(&ap, &current, "na_cozinha", salaAtual->getNome() == "Cozinha");
	goap_worldstate_set(&ap, &current, "armado", inventario.temItem("Faca"));

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Jenna::tomarAcao() {
	string acao = plan[currentStep];

	if (acao == "mover_cozinha") {
		seguirCaminho();

		if (caminho.size() == 0)
			avancarPlanos();
	}

	else if (acao == "procurar_alvo") {
		seguirCaminho();

		if (caminho.size() == 0)
			avancarPlanos();
	}

	else if (acao == "pegar_faca") {
		salaAtual->getObjeto("Faca")->takeAction("obter", nome);
		if (inventario.temItem("Faca"))
			avancarPlanos();
	}

	else if (acao == "matar") {
		attack(alvos[0]);
		goap_worldstate_set(&ap, &current, (alvos[0] + "_vivo").c_str(), false);
		avancarPlanos();
	}
}


void Jenna::avancarPlanos() {
	currentStep++;

	if (currentStep >= plansz)
		updatePlanos();

	if (plan[currentStep] == "mover_cozinha") {
		caminho = descobrirCaminho(mapa->getSala("Cozinha"));
	}

	else if (plan[currentStep] == "procurar_alvo") {
		if (ultimoAvistamento.hasKey(alvos[0]))
			caminho = procurar(mapa->getSala(ultimoAvistamento.getValues(alvos[0])));
		else
			caminho = procurar();

	}
}
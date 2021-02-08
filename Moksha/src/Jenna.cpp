#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna", F, 1, 1} {
	alvo = "Colt";
}

void Jenna::setupPlanos() {
	goap_actionplanner_clear(&ap); // initializes action planner

	// describe repertoire of actions
	goap_set_pst(&ap, "mover_cozinha", "na_cozinha", true);
	goap_set_cost(&ap, "mover_cozinha", tamanhoCaminho(salaAtual, mapa->getSala("Cozinha")));

	goap_set_pre(&ap, "pegar_faca", "na_cozinha", true);
	goap_set_pst(&ap, "pegar_faca", "armado", true);

	goap_set_pre(&ap, "seguir_jogador", "armado", true);
	goap_set_pst(&ap, "seguir_jogador", "com_jogador", true);
	goap_set_cost(&ap, "seguir_jogador", 2);

	goap_set_pre(&ap, "matar", "com_jogador", true);
	goap_set_pre(&ap, "matar", "armado", true);
	goap_set_pst(&ap, "matar", "jogador_vivo", false);


	// describe current world state.
	goap_worldstate_clear(&current);
	goap_worldstate_set(&ap, &current, "na_cozinha", salaAtual->getNome() == "Cozinha");
	goap_worldstate_set(&ap, &current, "com_jogador", false);
	goap_worldstate_set(&ap, &current, "armado", inventario.temItem("Faca"));
	goap_worldstate_set(&ap, &current, "jogador_vivo", true);

	// describe goal
	goap_worldstate_clear(&goal);
	goap_worldstate_set(&ap, &goal, "jogador_vivo", false);
	//goap_worldstate_set( &ap, &goal, "alive", true ); // add this to avoid suicide actions in the plan.

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
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

	else if (acao == "seguir_jogador") {
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
		attack(alvo);
		goap_worldstate_set(&ap, &current, "jogadorVivo", false);
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

	else if (plan[currentStep] == "seguir_jogador") {
		if (ultimoAvistamento.hasKey(alvo))
			caminho = procurar(mapa->getSala(ultimoAvistamento.getValues(alvo)));
		else
			caminho = procurar();

	}
}
#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna", F, 1, 1} {
	alvos.push_back("Elliot");
}

void Jenna::setupAcoesAdicional() {
	goap_set_pst(&ap, "move_kitchen", "in_kitchen", true);

	goap_set_pre(&ap, "take_knife", "in_kitchen", true);
	goap_set_pst(&ap, "take_knife", "armed", true);

	goap_set_pre(&ap, "search_Elliot", "armed", true);
	//goap_set_cost(&ap, "search_Elliot", 5);

	goap_set_pre(&ap, "kill", "armed", true);
	goap_set_pre(&ap, "kill", "with_Elliot", true);
	goap_set_pst(&ap, "kill", "Elliot_alive", false);
}


void Jenna::setupMundoAdicional() {

}


void Jenna::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &currentGoal.goal, "Elliot_alive", false);
	goap_worldstate_set(&ap, &currentGoal.goal, "Jenna_alive", true);
}


void Jenna::updateWorldExtra() {
	// describe current world state.
	goap_worldstate_set(&ap, &world, "in_kitchen", salaAtual->getNome() == "Kitchen");
	goap_worldstate_set(&ap, &world, "armed", inventory.hasItem("Knife"));

	goap_set_cost(&ap, "move_kitchen", tamanhoCaminho(salaAtual, mapa->getSala("Kitchen")));
}


int Jenna::decidirAcaoAdicional(string acao) {
	if (acao == "move_kitchen" || acao.substr(0, 7).compare("search_") == 0) {
		actionArgs.push_back(nextRoomInPath());
		return mover;
	}

	else if (acao == "take_knife") {
		actionArgs.push_back("take");
		actionArgs.push_back("Knife");
		return interagir;
	}

	else if (acao == "kill") {
		actionArgs.push_back(alvos[0]);
		return atacar;
	}

	return descansar;
}


void Jenna::advancePlansExtra(string currentProcess) {
	if (currentProcess == "move_kitchen")
		path = findPath(mapa->getSala("Kitchen"));
}
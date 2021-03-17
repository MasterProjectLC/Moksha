#include "Santos.h"

Santos::Santos(Mapa* m) : NPC{ m, "Santos", "A big ass dude.", F, 3, 2 } {
	trackablePeople.insert("Hilda");
	trackablePeople.insert("Baxter");
	trackablePeople.insert("Jenna");
}

void Santos::setupAcoesAdicional() {
	goap_set_pre(&ap, "talk_no_gun_Hilda", "with_Hilda", true);
	goap_set_pst(&ap, "talk_no_gun_Hilda", "talk_no_gun", true);

	goap_set_pre(&ap, "talk_no_gun_Baxter", "with_Baxter", true);
	goap_set_pst(&ap, "talk_no_gun_Baxter", "talk_no_gun", true);
}


void Santos::setupMundoAdicional() {
	goap_worldstate_set(&ap, &world, "with_Hilda", true);
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
	goap_worldstate_set(&ap, &world, "talk_no_gun", false);
}


void Santos::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &currentGoal.goal, "talk_no_gun", true);
}


int Santos::decidirAcaoAdicional(string acao) {
	if (acao == "talk_no_gun_Hilda" || acao == "talk_no_gun_Baxter") {
		actionArgs.push_back("santos_gun");
		goap_worldstate_set(&ap, &world, "talk_no_gun", true);
		return conversar;
	}
	return descansar;

}


void Santos::advancePlansExtra(string currentProcess) {

}


void Santos::updateWorldExtra() {

}
#include "Willow.h"

Willow::Willow(Map* m) : NPC{ m, "Willow", 
("Willow Blakewell, formerly known as Willow Niedenberg, is Baxter's wife, Liz' sister and a member of the austrian nobility. "
"As far as I know, she seems to be a compassionate and kind individual.\n"
"Her hair is long and elaborate, tranced in an intricate fashion. Today, she's wearing a golden and blue dress.")
, F, 1, 1 } {
	trackablePeople.insert("Baxter");
}

void Willow::setupActionsParticular() {
	addTrackableRoom("BlakewellRoom");
	goap_set_pre(&ap, "write", "in_BlakewellRoom", true);
	goap_set_pst(&ap, "write", "writing", true);
}


void Willow::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_BlakewellRoom", true);
	goap_worldstate_set(&ap, &world, "writing", false);
}


void Willow::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
}

void Willow::updateWorldExtra() {
	// describe current world state.
	
}

void Willow::updateProcessExtra(string currentProcess) {

}

int Willow::decideActionParticular(string action) {
	if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	return descansar;
}
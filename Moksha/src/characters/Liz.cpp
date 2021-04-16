#include "Liz.h"

Liz::Liz(Map* m) : NPC(m, "Liz") {

}

void Liz::setupActionsParticular() {
	addTrackablePeople("Santos");
	addTrackableRoom("LizRoom");
	goap_set_pre(&ap, "paint", "in_LizRoom", true);
	goap_set_pst(&ap, "paint", "painting", true);
}


void Liz::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_LizRoom", true);
	goap_worldstate_set(&ap, &world, "painting", false);
}


void Liz::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "painting", true);
}


void Liz::updateWorldExtra() {

}


void Liz::setupProcessParticular(string currentProcess) {

}


int Liz::decideActionParticular(string action) {
	if (action == "paint") {
		actionArgs.push_back("painting.");
		return acaoNula;
	}

	return descansar;
}
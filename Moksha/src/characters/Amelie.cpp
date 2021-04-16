#include "Amelie.h"

Amelie::Amelie(Map* m) : NPC( m, "Amelie" ) {

}

void Amelie::setupActionsParticular() {
	addTrackableRoom("AmelieRoom");
	addTrackableRoom("ViewingLobby");
	goap_set_pre(&ap, "meditate", "in_AmelieRoom", true);
	goap_set_pst(&ap, "meditate", "meditating", true);

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "medusa", true);
}


void Amelie::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "meditating", false);
	goap_worldstate_set(&ap, &world, "medusa", false);
}


void Amelie::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "meditating", true);
}

void Amelie::updateWorldExtra() {
	
}

int Amelie::decideActionParticular(string action) {
	if (action == "meditate") {
		actionArgs.push_back("meditating.");
		return acaoNula;
	}

	if (action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	return descansar;
}


void Amelie::setupProcessParticular(string currentProcess) {

}
#include "Amelie.h"

Amelie::Amelie(Map* m) : NPC( m, "Amelie" ) {

}

void Amelie::setupActionsParticular() {
	addTrackableRoom("AmelieRoom");
	addTrackableRoom("ViewingLobby");
	addTrackableNull("meditate", "meditating", "AmelieRoom");

	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");
}


void Amelie::setupWorldParticular() {

}


void Amelie::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "meditating", true);
}

void Amelie::updateWorldExtra() {
	
}

int Amelie::decideActionParticular(string action) {
	return descansar;
}


void Amelie::setupProcessParticular(string currentProcess) {

}
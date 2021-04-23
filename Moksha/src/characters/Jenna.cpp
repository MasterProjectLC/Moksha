#include "Jenna.h"

Jenna::Jenna(Map* m) : NPC(m, "Jenna") {

}

void Jenna::setupActionsParticular() {
	addTrackablePeople("Elliot");
	addTrackablePeople("Magnus");
	addTrackablePeople("Renard");
	addTrackablePeople("George");

	addTrackableRoom("JennaRoom");
	addTrackableNull("write", "writing", "JennaRoom");

	addTrackableConvo("wake_up");
	goap_set_pre(&ap, "convo_wake_up", "in_JennaRoom", true);
	goap_set_pre(&ap, "convo_wake_up", "in_JennaRoom", true);

	addTrackableRoom("ViewingLobby");
	addTrackableConvo("renard_questionnaire", "in_ViewingLobby");
	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");

	addTrackableRoom("Runway");
	addTrackableNull("wait_Runway", "waiting_Runway", "waiting", "Runway");

	addTrackableRoom("EntranceLounge");
	addTrackableConvo("jenna_amelie", "in_EntranceLounge");

	addTrackableRoom("GameRoom");
	addTrackableNull("play", "playing", "GameRoom");
}


void Jenna::setupWorldParticular() {

}


void Jenna::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
	addGoal(new string("convo_wake_up"), true, 5);
}


void Jenna::updateWorldExtra() {

}


void Jenna::setupProcessParticular(string currentProcess) {

}


int Jenna::decideActionParticular(string action) {
	return descansar;
}
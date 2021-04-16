#include "Jenna.h"

Jenna::Jenna(Map* m) : NPC(m, "Jenna") {

}

void Jenna::setupActionsParticular() {
	addTrackablePeople("Elliot");
	addTrackablePeople("Magnus");
	addTrackablePeople("Renard");
	addTrackablePeople("George");

	addTrackableRoom("JennaRoom");
	goap_set_pre(&ap, "write", "in_JennaRoom", true);
	goap_set_pst(&ap, "write", "writing", true);

	addTrackableConvo("wake_up");
	goap_set_pre(&ap, "convo_wake_up", "in_JennaRoom", true);

	addTrackableRoom("ViewingLobby");
	addTrackableConvo("renard_questionnaire", "ViewingLobby");

	addTrackableRoom("Runway");
	goap_set_pre(&ap, "wait_Runway", "in_Runway", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "the_medusa", true);

	addTrackableRoom("GameRoom");
	addTrackableConvo("begin_card_game", "GameRoom");
	goap_set_pre(&ap, "convo_wake_up", "in_JennaRoom", true);
	goap_set_pre(&ap, "play", "in_GameRoom", true);
	goap_set_pst(&ap, "play", "playing", true);
}


void Jenna::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	goap_worldstate_set(&ap, &world, "writing", false);
	goap_worldstate_set(&ap, &world, "the_medusa", false);
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
	if (action == "wait_Runway" || action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	if (action == "play") {
		actionArgs.push_back("playing.");
		return acaoNula;
	}

	return descansar;
}
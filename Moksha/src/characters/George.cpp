#include "George.h"

George::George(Map* m) : NPC(m, "George") {

}

void George::setupActionsParticular() {
	addTrackablePeople("Jenna");
	addTrackablePeople("Willow");
	addTrackableRoom("Runway");
	addTrackableRoom("ViewingLobby");
	addTrackableRoom("DiningHall");
	goap_set_pre(&ap, "wait_Runway", "in_Runway", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);
	goap_set_pre(&ap, "have_breakfast", "in_DiningHall", true);
	goap_set_pst(&ap, "have_breakfast", "breakfast", true);

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "the_medusa", true);

	addTrackableRoom("GameRoom");
	addTrackableConvo("begin_card_game", "GameRoom");
	goap_set_pre(&ap, "wait_Jenna", "expecting_Jenna", true);
	goap_set_pst(&ap, "wait_Jenna", "with_Jenna", true);
	goap_set_cost(&ap, "wait_Jenna", 0);
	goap_set_pre(&ap, "play", "in_GameRoom", true);
	goap_set_pst(&ap, "play", "playing", true);
}


void George::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "breakfast", false);
	goap_worldstate_set(&ap, &world, "in_Runway", false);
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	goap_worldstate_set(&ap, &world, "the_medusa", false);
	goap_worldstate_set(&ap, &world, "expecting_Jenna", true);
	updateWorldExtra();
}


void George::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "breakfast", true);
}


void George::updateWorldExtra() {
	// describe current world state.
	if (lastSeen.hasKey("Jenna"))
		goap_set_cost(&ap, "search_Jenna", pathSize(currentRoom, mapp->getRoom(lastSeen.getValues("Jenna"))));
}


void George::setupProcessParticular(string currentProcess) {

}


int George::decideActionParticular(string action) {
	if (action == "wait_Runway" || action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	if (action == "have_breakfast") {
		actionArgs.push_back("having breakfast.");
		return acaoNula;
	}

	if (action == "play") {
		actionArgs.push_back("playing.");
		return acaoNula;
	}

	return descansar;
}
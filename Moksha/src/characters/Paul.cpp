#include "Paul.h"

Paul::Paul(Map* m) : NPC(m, "Paul") {

}

void Paul::setupActionsParticular() {
	addTrackablePeople("Baxter");
	addTrackablePeople("Santos");
	addTrackablePeople("Magnus", vector<string>({"MagnusRoom", "Study"}));
	addTrackableRoom("RenardRoom");
	addTrackableRoom("ViewingLobby");
	addTrackableRoom("Study");
	addTrackableRoom("SantosRoom");
	setupCrewArea();

	goap_set_pre(&ap, "take_gun", "in_SantosRoom", true);
	goap_set_pre(&ap, "take_gun", "santos_gun_exists", true);
	goap_set_pst(&ap, "take_gun", "with_gun", true);

	goap_set_pre(&ap, "listen_santos", "listen_Santos", true);
	goap_set_pst(&ap, "listen_santos", "santos_gun_exists", true);

	addTrackableNull("plan", "planning", "RenardRoom");
	addTrackableNull("watch_workers", "watching_workers", "watching the workers", "DiningHall");
	addTrackableConvo("airship_design", "with_Magnus");

	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");
}


void Paul::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "santos_gun_exists", false);
}


void Paul::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "planning", true);
}


void Paul::updateWorldExtra() {
	// describe current world state.
}

void Paul::setupProcessParticular(string currentProcess) {

}

int Paul::decideActionParticular(string action) {
	if (action == "enter_CrewArea") {
		actionArgs.push_back("open");
		actionArgs.push_back("Crew Door");
		setCondition("in_CrewArea", true);
		return interagir;
	}
	if (action == "leave_CrewArea") {
		actionArgs.push_back("open");
		actionArgs.push_back("Crew Door");
		setCondition("in_CrewArea", false);
		return interagir;
	}
	if (action == "listen_Santos") {
		actionArgs.push_back("Santos");
		return ouvir;
	}
	if (action == "take_gun") {
		actionArgs.push_back("take");
		actionArgs.push_back("Santos' gun");
		setCondition("with_gun", true);
		return interagir;
	}

	return descansar;
}
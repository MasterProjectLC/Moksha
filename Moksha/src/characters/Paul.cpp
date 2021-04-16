#include "Paul.h"

Paul::Paul(Map* m) : NPC(m, "Paul") {

}

void Paul::setupActionsParticular() {
	addTrackablePeople("Baxter");
	addTrackablePeople("Santos");
	addTrackablePeople("Magnus");
	addTrackableRoom("RenardRoom");
	addTrackableRoom("ViewingLobby");
	setupCrewArea();

	goap_set_pre(&ap, "plan", "in_RenardRoom", true);
	goap_set_pst(&ap, "plan", "planning", true);

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "medusa", true);
}


void Paul::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "planning", false);
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
	if (action == "plan") {
		actionArgs.push_back("planning.");
		return acaoNula;
	}

	if (action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

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

	return descansar;
}
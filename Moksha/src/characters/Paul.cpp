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
	setupCrewArea();

	addTrackableNull("plan", "planning", "RenardRoom");
	addTrackableConvo("airship_design", "with_Magnus");

	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");
}


void Paul::setupWorldParticular() {

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

	return descansar;
}
#include "Tom.h"

Tom::Tom(Map* m) : NPC(m, "Tom") {

}

void Tom::setupActionsParticular() {
	addTrackablePeople("Hilda");
	addTrackablePeople("Santos");
	setupCrewArea();

	goap_set_pre(&ap, "move_ControlRoom", "in_CrewArea", true);
	addTrackableRoom("ControlRoom");
	goap_set_pre(&ap, "move_NorthEngines", "in_CrewArea", true);
	addTrackableRoom("NorthEngines");
	goap_set_pre(&ap, "move_CrewQuarters", "in_CrewArea", false);
	addTrackableRoom("CrewQuarters");

	goap_set_pre(&ap, "work", "in_NorthEngines", true);
	goap_set_pst(&ap, "work", "working", true);
	addTrackableNull("work", "working", "NorthEngines");
}


void Tom::setupWorldParticular() {

}


void Tom::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "working", true);
}

void Tom::updateWorldExtra() {
	// describe current world state.
}

void Tom::setupProcessParticular(string currentProcess) {

}

int Tom::decideActionParticular(string action) {
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
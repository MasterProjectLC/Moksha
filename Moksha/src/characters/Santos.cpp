#include "Santos.h"

Santos::Santos(Map* m) : NPC(m, "Santos") {

}

void Santos::setupActionsParticular() {
	addTrackablePeople("Hilda");
	addTrackablePeople("Baxter");
	addTrackablePeople("Tom");
	addTrackableConvo("santos_gun");
	setupCrewArea();

	goap_set_pre(&ap, "move_Navigation", "in_CrewArea", true);
	addTrackableRoom("Navigation");

	goap_set_pre(&ap, "convo_santos_gun", "with_Baxter", true);
	goap_set_pre(&ap, "pilot", "in_Navigation", true);
	goap_set_pst(&ap, "pilot", "piloting", true);
}


void Santos::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "piloting", false);
}


void Santos::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "piloting", true);
	//goap_worldstate_set(&ap, &currentGoal.goal, "convo_santos_gun", true);
}


int Santos::decideActionParticular(string action) {
	if (action == "pilot") {
		actionArgs.push_back("piloting.");
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


void Santos::setupProcessParticular(string currentProcess) {

}


void Santos::updateWorldExtra() {

}
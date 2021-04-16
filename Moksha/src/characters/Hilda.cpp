#include "Hilda.h"

Hilda::Hilda(Map* m) : NPC(m, "Hilda") {
	
}

void Hilda::setupActionsParticular() {
	addTrackablePeople("Santos");
	addTrackablePeople("Baxter");
	addTrackablePeople("Tom");
	setupCrewArea();

	goap_set_pre(&ap, "move_Navigation", "in_CrewArea", true);
	addTrackableRoom("Navigation");
	goap_set_pre(&ap, "move_NorthEngines", "in_CrewArea", true);
	addTrackableRoom("NorthEngines");
	goap_set_pre(&ap, "move_CrewQuarters", "in_CrewArea", false);
	addTrackableRoom("CrewQuarters");

	goap_set_pre(&ap, "pilot", "in_Navigation", true);
	goap_set_pst(&ap, "pilot", "piloting", true);
	addTrackableConvo("hilda_speech", "NorthEngines");
	addTrackableConvo("hilda_sleep", "Navigation");
	goap_set_pre(&ap, "sleep", "in_CrewQuarters", true);
	goap_set_pst(&ap, "sleep", "sleeping", true);
}


void Hilda::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "sleeping", false);
	goap_worldstate_set(&ap, &world, "piloting", false);
	goap_worldstate_set(&ap, &world, "in_CrewArea", false);
}


void Hilda::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "sleeping", true);
	addGoal(new string("convo_hilda_speech"), true, 50);
	addGoal(new string("convo_hilda_sleep"), true, 49);
}


int Hilda::decideActionParticular(string action) {
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


void Hilda::setupProcessParticular(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}
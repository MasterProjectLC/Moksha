#include "Santos.h"

Santos::Santos(Map* m) : NPC(m, "Santos") {

}

void Santos::setupActionsParticular() {
	addTrackablePeople("Hilda");
	addTrackablePeople("Baxter");
	addTrackablePeople("Tom");
	addTrackableConvo("santos_gun", "with_Baxter");
	setupCrewArea();

	goap_set_pre(&ap, "move_Navigation", "in_CrewArea", true);
	addTrackableRoom("Navigation");

	goap_set_pre(&ap, "move_SantosRoom", "in_CrewArea", false);
	addTrackableRoom("SantosRoom");

	addTrackableConvo("tom_lunch", "in_DiningHall");
	addTrackableConvo("tom_slacking", "in_Navigation");
	addTrackableConvo("baxter_santos", "in_DiningHall");

	addTrackableNull("pilot", "piloting", "Navigation");
	addTrackableNull("train", "training", "SantosRoom");
}


void Santos::setupWorldParticular() {

}


void Santos::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "piloting", true);
	//goap_worldstate_set(&ap, &currentGoal.goal, "convo_santos_gun", true);
}


int Santos::decideActionParticular(string action) {
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
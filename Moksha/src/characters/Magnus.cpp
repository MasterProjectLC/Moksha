#include "Magnus.h"

Magnus::Magnus(Map* m) : NPC(m, "Magnus") {

}

void Magnus::setupActionsParticular() {
	addTrackablePeople("Amelie", vector<string>({ "AmelieRoom" }));
	addTrackablePeople("Baxter");
	addTrackablePeople("Santos");
	addTrackableRoom("Study");
	addTrackableRoom("MagnusRoom");
	addTrackableRoom("AmelieRoom");
	addTrackableRoom("ViewingLobby");

	addTrackableRoom("AmelieRoom");
	addTrackableConvo("amelie_magnus", "with_Amelie");

	addTrackableNull("study", "studying", "Study");
	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");
}


void Magnus::setupWorldParticular() {

}


void Magnus::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "studying", true);
}


void Magnus::updateWorldExtra() {
	// describe current world state.
}

void Magnus::setupProcessParticular(string currentProcess) {

}

int Magnus::decideActionParticular(string action) {
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
#include "Magnus.h"

Magnus::Magnus(Map* m) : NPC(m, "Magnus") {

}

void Magnus::setupActionsParticular() {
	addTrackablePeople("Amelie");
	addTrackablePeople("Baxter");
	addTrackablePeople("Santos");
	addTrackableRoom("Study");
	addTrackableRoom("MagnusRoom");
	addTrackableRoom("ViewingLobby");
	goap_set_pre(&ap, "study", "in_Study", true);
	goap_set_pst(&ap, "study", "studying", true);

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "the_medusa", true);
}


void Magnus::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "studying", false);
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
	if (action == "study") {
		actionArgs.push_back("studying.");
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
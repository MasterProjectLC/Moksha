#include "Baxter.h"

Baxter::Baxter(Map* m) : NPC(m, "Baxter") {

}

void Baxter::setupActionsParticular() {
	addTrackablePeople("Paul");
	addTrackablePeople("Willow");
	addTrackablePeople("Renard");
	addTrackablePeople("Santos");
	addTrackablePeople("Hilda");

	setupCrewArea();
	addTrackableRoom("BlakewellRoom");
	goap_set_pre(&ap, "prepare", "in_BlakewellRoom", true);
	goap_set_pst(&ap, "prepare", "preparing", true);

	addTrackableRoom("ViewingLobby");
	addTrackableConvo("baxter_facade", "BlakewellRoom");
	addTrackableConvo("presentation", "ViewingLobby");
	goap_set_pre(&ap, "convo_presentation", "convo_baxter_facade", true);
	addTrackableConvo("familiar_faces", "ViewingLobby");
	goap_set_pre(&ap, "convo_familiar_faces", "convo_presentation", true);
}


void Baxter::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "preparing", false);
	goap_worldstate_set(&ap, &world, "medusa", false);
}


void Baxter::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "preparing", true);
}


void Baxter::updateWorldExtra() {

}


void Baxter::setupProcessParticular(string currentProcess) {

}


int Baxter::decideActionParticular(string action) {
	if (action == "prepare") {
		actionArgs.push_back("preparing speeches.");
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
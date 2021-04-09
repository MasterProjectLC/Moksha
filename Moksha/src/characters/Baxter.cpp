#include "Baxter.h"

Baxter::Baxter(Map* m) : NPC{ m, "Baxter", 
("Baxter Blakewell is one of the original founders of the Blakewell and Wright Aviation Company. "
"He's a posh englishman known for his somewhat cold demeanor and conservative business moves. In regards to personal life, he's married to Willow Blakewell.\n"
"Baxter has an average build and sports a simple moustache. Right now, he's wearing a sharp, elegant double suit and a black striped tie.")
, M, 5, 7 } {

}

void Baxter::setupActionsParticular() {
	addTrackablePeople("Paul");
	addTrackablePeople("Willow");
	addTrackablePeople("Renard");
	addTrackablePeople("Santos");
	addTrackablePeople("Hilda");
	addTrackableRoom("BlakewellRoom");
	addTrackableRoom("ViewingLobby");
	addTrackableConvo("baxter_facade", "BlakewellRoom");
	addTrackableConvo("presentation", "ViewingLobby");
	goap_set_pre(&ap, "convo_presentation", "convo_baxter_facade", true);
}


void Baxter::setupWorldParticular() {

}


void Baxter::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "in_BlakewellRoom", true);
	addGoal(new string("convo_presentation"), true, 5);
}


void Baxter::updateWorldExtra() {

}


void Baxter::setupProcessParticular(string currentProcess) {

}


int Baxter::decideActionParticular(string action) {
	return descansar;
}
#include "Baxter.h"

Baxter::Baxter(Map* m) : NPC{ m, "Baxter", 
("Baxter Blakewell is one of the original founders of the Blakewell and Wright Aviation Company. "
"He's a posh englishman known for his somewhat cold demeanor and conservative business moves. In regards to personal life, he's married to Willow Blakewell.\n"
"Baxter has an average build and sports a simple moustache.Right now, he's wearing a sharp, elegant double suit and a black striped tie.")
, M, 5, 7 } {
	trackablePeople.insert("Elliot");
	trackablePeople.insert("Santos");
}

void Baxter::setupActionsParticular() {
	addTrackableRoom("BlakewellRoom");
}


void Baxter::setupWorldParticular() {

}


void Baxter::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "in_BlakewellRoom", true);
}


void Baxter::updateWorldExtra() {

}


void Baxter::updateProcessExtra(string currentProcess) {

}


int Baxter::decideActionParticular(string action) {
	return descansar;
}
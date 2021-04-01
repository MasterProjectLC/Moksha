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
	goap_set_pst(&ap, "move_Runway", "in_BlakewellRoom", true);
	goap_set_pre(&ap, "wait_Runway", "in_BlakewellRoom", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);
}


void Baxter::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_BlakewellRoom", false);
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	updateWorldExtra();
}


void Baxter::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "waiting_Runway", true);
	//addGoal(new string("in_Runway"), true);
}


void Baxter::updateWorldExtra() {
	// describe current world state.
	goap_worldstate_set(&ap, &world, "in_BlakewellRoom", currentRoom->getCodename() == "BlakewellRoom");
}


void Baxter::advancePlansExtra(string currentProcess) {
	if (currentProcess == "move_BlakewellRoom")
		path = findPath(mapp->getRoom("BlakewellRoom"));
}


int Baxter::decideActionParticular(string action) {
	if (action == "wait_Runway") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	return descansar;
}
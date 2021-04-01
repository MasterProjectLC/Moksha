#include "npc.h"
#include <stdexcept>

NPC::NPC(Map* m, string name, string description, int gender, int strength, int dexterity) : Character(gender, strength, dexterity) {
	this->name = name;
	this->description = new string(description);
	this->mapp = m;
	busy = false;

	FileDict fileObject = FileManager::readFromFile("files/characters/" + getName() + ".txt");
	this->dict = fileObject;
}


NPC::~NPC() {
	clear();
}

void NPC::clear() {
	inventory.clear();
	goalList.clear();
	//currentGoal = Goal();
	checkedRooms.clear();
	statusEffects.clear();

	for (set<string*>::iterator it = addedConditions.begin(); it != addedConditions.end(); it++) {
		delete *it;
	}
	addedConditions.clear();

	for (set<string*>::iterator it = addedActions.begin(); it != addedActions.end(); it++) {
		delete *it;
	}
	addedActions.clear();

	for (int i = 0; i < conditionNames.size(); i++) {
		delete conditionNames[i];
	}
	conditionNames.clear();
	trackablePeople.clear();
	while (!path.empty())
		path.pop();
	lastSeen.clear();
}

// PATHFINDING ---------------

queue<Room*> NPC::findPath(Room* targetRoom) {
	return findPath(currentRoom, targetRoom);
};

queue<Room*> NPC::findPath(Room* initialRoom, Room* targetRoom) {
	return mapp->optimalPath(initialRoom, targetRoom);
};

queue<Room*> NPC::search() {
	return search(currentRoom);
};

queue<Room*> NPC::search(Room* roomClue) {
	// Path to the clue
	queue<Room*> retorno;
	retorno.push(currentRoom);
	if (roomClue != currentRoom)
		retorno = findPath(roomClue);

	// Start search
	queue<Room*> search = mapp->breadthSearch(roomClue);
	while (!search.empty()) {
		queue<Room*> path = findPath(retorno.back(), search.front());
		while (!path.empty()) {
			retorno.push(path.front());
			path.pop();
		}
		search.pop();
	}

	if (retorno.front() == currentRoom)
		retorno.pop();
	return retorno;
};


string NPC::nextRoomInPath() {
	string retorno = "";
	if (path.empty())
		advancePlansExtra(plan[currentStep]); // Refresh plans

	if (!path.empty()) {
		retorno = path.front()->getCodename();
		path.pop();
	}

	return retorno;
}

// REACTIONS ----------------------------------

void NPC::executeReaction(string topic, string phrase, string sender, bool shouldRespond) {
	if (isUnconscious())
		return;

	if (topic != "")
		setCondition(topic, true);
	// React to mentions
	if (dict.hasKey(topic) && shouldRespond && !busy && !inConversation()) {
		talk(dict.getValue(topic), true);
		notify(avancar);
	}
}


void NPC::setCondition(string condition, bool update) {
	if (!hasCondition(condition)) {
		string *pointer = new string(condition);
		addedConditions.insert(pointer);
		goap_worldstate_set(&ap, &world, pointer->c_str(), update);
	}
	else
		goap_worldstate_set(&ap, &world, condition.c_str(), update);
}


void NPC::receiveEvent(vector<string> args) {
	if (args[0] == "attack")
		if (trackablePeople.count(args[1]) > 0)
			setCondition(args[1] + "_dead", true);
}


void NPC::checkRoom(vector<Character*> peopleInRoom) {
	// Run through tracked people
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++) {
		string theirName = *it;
		goap_worldstate_set(&ap, &world, ("with_" + theirName).c_str(), false);

		// Check if they are in the room
		for (int i = 0; i < peopleInRoom.size(); i++) {
			if (theirName == peopleInRoom[i]->getName()) {
				updateLastSeen(theirName, currentRoom->getCodename());
				goap_worldstate_set(&ap, &world, ("with_" + theirName).c_str(), true);
				break;
			}
		}
	}

	updateWorld();
};


void NPC::seeCharMoving(Character* person, Room* otherRoom, bool entering) {
	if (trackablePeople.count(person->getName()) == 0)
		return;

	if (!entering)
		updateLastSeen(person->getName(), otherRoom->getCodename());
	else
		updateLastSeen(person->getName(), currentRoom->getCodename());

	goap_worldstate_set(&ap, &world, ("with_" + person->getName()).c_str(), entering);
	updateWorld();
}


// PLANS -------------------------------------------

void NPC::setupPlans() {
	goap_actionplanner_clear(&ap); // initializes action planner

	// describe repertoire of actions
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++) {
		setCondition("with_" + *it, false);
	}
	setupActionsParticular();

	// describe current world state.
	goap_worldstate_clear(&world);
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++)
		setCondition("with_" + *it, false);
	setupWorldParticular();

	// describe goal
	goalList = PriorityVector<Goal>(vector<Goal>(), goalCompare);

	currentGoal = Goal(0, false);
	goap_worldstate_clear(&currentGoal.goal);
	setupObjectivesParticular();
	goalList.push(currentGoal);
	goalList.sort();

	// calculate initial plan
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	updateWorld();
	advancePlans();
}


void NPC::addGoal(vector<string*> conditions, vector<bool> conditionStates) {
	if (conditions.size() != conditionStates.size())
		return;
	Goal newgoal = Goal(1, true);
	goap_worldstate_clear(&newgoal.goal);
	for (int i = 0; i < conditions.size(); i++) {
		goap_worldstate_set(&ap, &newgoal.goal, conditions[i]->c_str(), conditionStates[i]);
		conditionNames.push_back(conditions[i]);
	}
	goalList.push(newgoal);
}

void NPC::addGoal(string* condition, bool conditionState) {
	Goal newgoal = Goal(1, true);
	goap_worldstate_clear(&newgoal.goal);
	goap_worldstate_set(&ap, &newgoal.goal, condition->c_str(), conditionState);
	conditionNames.push_back(condition);
	goalList.push(newgoal);
}


void NPC::updateWorld() {
	updateWorldExtra();

	// Detect that current action is impossible
	if (plansz > 0) {
		bfield_t prereqs;
		goap_get_pre(&ap, plan[currentStep], &prereqs);
		if (prereqs & world.values != prereqs)
			changePlans(true);
	}
}


void NPC::updateLastSeen(string pursueTarget, string room) {
	string alvoVec[1] = { pursueTarget };
	lastSeen.addPair(set<string>(alvoVec, alvoVec + 1), room);

	if (plansz == 0)
		return;

	string currentProcess = plan[currentStep];
	if (currentProcess.compare("search_" + pursueTarget) == 0) {
		path = search(mapp->getRoom( lastSeen.getValues(pursueTarget) ));
	}
}


void NPC::advancePlans() {
	currentStep++;

	// The plan has come to its end
	if (currentStep >= plansz && plansz > 0)
		changePlans();

	// Advance plans
	if (plansz > 0) {
		string currentProcess = plan[currentStep];

		if (currentProcess.substr(0, 7).compare("search_") == 0) {
			if (lastSeen.hasKey( currentProcess.substr(7, 10000) ))
				path = search(mapp->getRoom(lastSeen.getValues( currentProcess.substr(7, 10000) )));
			else
				path = search();
		}
		else if (currentProcess.substr(0, 5).compare("move_") == 0) {
			path = findPath(mapp->getRoom( currentProcess.substr(5, 10000) ));
		} else
			advancePlansExtra(currentProcess);
	}
}


void NPC::changePlans(bool justUpdated) {
	if (!justUpdated)
		updateWorldExtra();

	// Calculate a new plan
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	advancePlans();
}


// ACTIONS ------------------------------------------------

int NPC::decideAction() {
	// Check if current action was completed
	while (isCurrentStateFulfilled() && plansz > 0)
		advancePlans();

	// Current objective is top priority
	if (currentGoal.goal.values == (goalList.highest()->goal.values)) {
		// Current objective was completed - find next non-completed objective
		vector<Goal>::iterator it = goalList.highest();
		if ( ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values || currentStep >= plansz) && !goalList.empty() ) {
			do {
				if (it->onetime)
					goalList.getVector().erase(it);

				goalList.descend(it);
				currentGoal = *it;
				currentStep = 0;
			} while ( (world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest() );
			changePlans();
		}
	
	// Current objective is not top priority - find next non-completed objective
	} else {
		vector<Goal>::iterator it = goalList.highest();
		do {
			currentGoal = *it;
			currentStep = 0;
			goalList.descend(it);
		} while ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest());
		changePlans();
	}

	// Decide action
	busy = false;
	if (plansz > 0) {
		actionArgs.clear();
		string action = plan[currentStep];

		if ( action.substr(0, 5).compare("move_") == 0 || action.substr(0, 7).compare("search_") == 0 ) {
			actionArgs.push_back(nextRoomInPath());
			currentAction = mover;
		} else 
			currentAction = decideActionParticular(action);
	} else
		currentAction = descansar;

	return currentAction;
}


// HELPER ----------------------------------------

bool NPC::isCurrentStateFulfilled() {
	bfield_t worldState = (world.values & ~states[currentStep].dontcare);
	bfield_t objState = states[currentStep].values;
	if (currentStep > 0)
		objState = states[currentStep].values & ~states[currentStep-1].values;
	bool fulfilled = ((worldState & objState) == objState);
	if (fulfilled && currentStep > 0) {
		objState = states[currentStep].values | ~states[currentStep - 1].values;
		fulfilled = fulfilled && ((worldState | objState) == objState);
	}

	return fulfilled;
}


bool NPC::hasCondition(string info) {
	bool retorno;
	if (!goap_worldstate_get(&ap, &world, info.c_str(), &retorno))
		return false;
	return retorno;
}


// GETTER
vector<string> NPC::getActionList() {
	vector<string> retorno;
	for (int i = 0; i < ap.numactions; i++)
		retorno.push_back(ap.act_names[i]);
	return retorno;
}


vector<string> NPC::getAtomList() {
	vector<string> retorno;
	for (int i = 0; i < ap.numatoms; i++)
		retorno.push_back(ap.atm_names[i]);
	return retorno;
}
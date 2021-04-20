#include "npc.h"
#include <stdexcept>

NPC::NPC(Map* m, string name) {
	this->mapp = m;
	busy = false;

	FileDict fileObject = FileManager::readFromFile("files/characters/" + name + ".txt");
	this->name = name;
	this->description = new string(fileObject.getValue("description"));
	this->gender = fileObject.getValue("gender")[0];
	this->strength = stoi(fileObject.getValue("strength"));
	this->dexterity = stoi(fileObject.getValue("dexterity"));
	vector<string> v = fileObject.getValues("topics");
	this->topicList = set<string>(v.begin(), v.end());

	goalList = PriorityVector<Goal>(vector<Goal>(), goalCompare);
}


NPC::~NPC() {
	clear();
}

void clearHelper(set<string*> *toClear) {
	for (set<string*>::iterator it = toClear->begin(); it != toClear->end(); it++) {
		delete *it;
	}
	toClear->clear();
}

void NPC::clear() {
	inventory.clear();
	goalList.clear();
	//currentGoal = Goal();
	checkedRooms.clear();
	statusEffects.clear();

	trackablePeople.clear();
	clearHelper(&trackableRooms);
	clearHelper(&trackableConvos);
	clearHelper(&addedConditions);
	clearHelper(&addedActions);

	for (int i = 0; i < conditionNames.size(); i++) {
		delete conditionNames[i];
	}
	conditionNames.clear();
	trackablePeople.clear();
	while (!path.empty())
		path.pop();
	lastSeen.clear();
}


void NPC::addTrackableRoom(string room) {
	string* novo = new string(room);
	trackableRooms.insert(novo);
	string* move_novo = new string("move_" + *novo);
	string* in_novo = new string("in_" + *novo);
	addedActions.insert(move_novo);
	addedConditions.insert(in_novo);
	goap_set_pst(&ap, move_novo->c_str(), in_novo->c_str(), true);
}

void NPC::addTrackablePeople(string person) {
	string* novo = new string(person);
	trackablePeople.insert(person);
	string* search_novo = new string("search_" + *novo);
	string* with_novo = new string("with_" + *novo);
	addedActions.insert(search_novo);
	addedConditions.insert(with_novo);
	goap_set_pst(&ap, search_novo->c_str(), with_novo->c_str(), true);
}

void NPC::addTrackableConvo(string convo) {
	string* novo = new string(convo);
	trackableConvos.insert(novo);
	string* convo_novo = new string("convo_" + *novo);
	addedActions.insert(convo_novo);
	goap_set_pst(&ap, convo_novo->c_str(), convo_novo->c_str(), true);
}

void NPC::addTrackableConvo(string convo, string room) {
	string* novo = new string(convo);
	trackableConvos.insert(novo);
	string* convo_novo = new string("convo_" + *novo);
	addedActions.insert(convo_novo);
	goap_set_pst(&ap, convo_novo->c_str(), convo_novo->c_str(), true);

	set<string*>::iterator it;
	for (it = addedConditions.begin(); it != addedConditions.end(); it++)
		if (**it == ("in_" + room))
			break;

	if (it != addedConditions.end())
		goap_set_pre(&ap, convo_novo->c_str(), (*it)->c_str(), true);
}

// PATHFINDING ==========================================

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


void NPC::advancePath() {
	if (path.empty())
		setupProcess(plan[currentStep]); // Refresh plans
	else
		path.pop();
}

void NPC::move(string room) {
	Character::move(room);
	advancePath();
}

void NPC::talk(string convo, bool isReaction) {
	for (set<string*>::iterator it = addedConditions.begin(); it != addedConditions.end(); it++)
		if (**it == "convo_" + convo)
			goap_worldstate_set(&ap, &world, (*it)->c_str(), true);
	Character::talk(convo, isReaction);
}

// REACTIONS ==========================================

void NPC::executeReaction(string topic, string phrase, string sender, bool shouldRespond) {
	if (isUnconscious())
		return;

	if (topic != "")
		setCondition(topic, true);
	// React to mentions
	if (shouldRespond && !busy && !inConversation()) {
		talk(topic + "_" + name, true);
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


void NPC::checkRoomParticular(vector<Character*> peopleInRoom) {
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


void NPC::seeCharMovingParticular(Character* person, Room* otherRoom, bool entering) {
	if (trackablePeople.count(person->getName()) == 0)
		return;

	if (!entering)
		updateLastSeen(person->getName(), otherRoom->getCodename());
	else
		updateLastSeen(person->getName(), currentRoom->getCodename());

	goap_worldstate_set(&ap, &world, ("with_" + person->getName()).c_str(), entering);
	updateWorld();
}


// PLANS ==================================================

// SETUP -----------------------------
void NPC::setupWorld() {
	goap_actionplanner_clear(&ap); // initializes action planner

	// describe repertoire of actions
	addTrackableRoom("DiningHall");
	setupActionsParticular();

	// describe current world state.
	goap_worldstate_clear(&world);
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++)
		setCondition("with_" + *it, false);
	for (set<string*>::iterator it = trackableConvos.begin(); it != trackableConvos.end(); it++)
		setCondition("convo_" + **it, false);
	setupWorldParticular();
	updateWorldVariables();

	// Describe default goal
	currentGoal = Goal(0, false);
	goap_worldstate_clear(&currentGoal.goal);
	setupObjectivesParticular();
	goalList.push(currentGoal);
	goalList.sort();

	// calculate initial plan
	createPlan();
}


void NPC::createPlan() {
	plansz = 16;
	stateZero = world;
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	advancePlans();
}


void NPC::addGoal(vector<string*> conditions, vector<bool> conditionStates, int priority) {
	if (conditions.size() != conditionStates.size())
		return;
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	for (int i = 0; i < conditions.size(); i++) {
		goap_worldstate_set(&ap, &newgoal.goal, conditions[i]->c_str(), conditionStates[i]);
		conditionNames.push_back(conditions[i]);
	}
	goalList.push(newgoal);
}

void NPC::addGoal(string* condition, bool conditionState, int priority) {
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	goap_worldstate_set(&ap, &newgoal.goal, condition->c_str(), conditionState);
	conditionNames.push_back(condition);
	goalList.push(newgoal);
}

void NPC::addGoal(bfield_t values, bfield_t dontcare, bool conditionState, int priority) {
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	newgoal.setGoal(values, dontcare);
	goalList.push(newgoal);
}


// EXTERNAL UPDATE -----------------------------
void NPC::updateWorld() {
	updateWorldVariables();

	// Detect that current action is impossible
	if (plansz > 0) {
		bfield_t prereqs;
		goap_get_pre(&ap, plan[currentStep], &prereqs);
		if (prereqs & world.values != prereqs)
			changePlans(true);
	}
}

void NPC::updateWorldVariables() {
	for (set<string*>::iterator it = trackableRooms.begin(); it != trackableRooms.end(); it++)
		goap_worldstate_set(&ap, &world, ("in_" + **it).c_str(), currentRoom->getCodename() == **it);

	updateWorldExtra();
}


void NPC::updateLastSeen(string pursueTarget, string room) {
	string alvoVec[1] = { pursueTarget };
	lastSeen.addPair(set<string>(alvoVec, alvoVec + 1), room);

	if (plansz == 0)
		return;

	string currentProcess = plan[currentStep];
	if (currentProcess.compare("search_" + pursueTarget) == 0)
		path = search(mapp->getRoom( lastSeen.getValues(pursueTarget) ));
}


// ADVANCEMENT ----------------------
void NPC::changePlans(bool justUpdated) {
	if (!justUpdated)
		updateWorldVariables();

	// Calculate a new plan
	createPlan();
}


void NPC::advancePlans() {
	currentStep++;
	// Current plan still going
	if (plansz > 0)
		if (currentStep < plansz)
			setupProcess(plan[currentStep]);

		// End of the line
		else
			changePlans();
}


void NPC::setupProcess(string currentProcess) {
	// Search - Setup path plan
	if (currentProcess.substr(0, 7).compare("search_") == 0) {
		if (lastSeen.hasKey(currentProcess.substr(7, 10000)))
			path = search(mapp->getRoom(lastSeen.getValues(currentProcess.substr(7, 10000))));
		else
			path = search();
	}
	// Move - setup path plan
	else if (currentProcess.substr(0, 5).compare("move_") == 0) {
		path = findPath(mapp->getRoom(currentProcess.substr(5, 10000)));
	}
	// Other actions that require prior planning
	else
		setupProcessParticular(currentProcess);
}


// ACTIONS =======================================

int NPC::decideAction() {
	goalList.sort();
	// Current objective is not top priority - find next non-completed objective
	if (currentGoal.goal.values != goalList.highest()->goal.values) {
		vector<Goal>::iterator it = goalList.highest();
		do {
			currentGoal = *it;
			currentStep = 0;
			goalList.descend(it);
		} while ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest());
		changePlans();

	// Current objective is top priority
	} else {
		// Current objective was completed - find next non-completed objective
		vector<Goal>::iterator it = goalList.highest();
		if ( ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values || currentStep >= plansz) && !goalList.empty() ) {
			do {
				if (it->onetime) {
					vector<Goal>::iterator dit = it;
					goalList.descend(it);
					goalList.erase(dit);
				} else
					goalList.descend(it);

				currentGoal = *it;
				currentStep = 0;
			} while ( (world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest() );
			changePlans();
		}
		// Current objective not completed - find next action
		else {
			while (isCurrentStateFulfilled() && plansz > 0)
				advancePlans();
		}
	}

	// Decide action
	if (plansz > 0) {
		actionArgs.clear();
		string action = plan[currentStep];

		// Move / Search
		if (action.substr(0, 5).compare("move_") == 0 || action.substr(0, 7).compare("search_") == 0) {
			actionArgs.push_back(path.front()->getCodename());
			currentAction = mover;
		}
		// Convo
		else if (action.substr(0, 6).compare("convo_") == 0) {
			actionArgs.push_back(action.substr(6, 1000));
			currentAction = conversar;
		// Other actions
		} else 
			currentAction = decideActionParticular(action);
	} else
		currentAction = descansar;

	busy = !(currentAction == descansar || currentAction == acaoNula);
	return currentAction;
}


// HELPER =========================================

bool NPC::isCurrentStateFulfilled() {
	bfield_t worldState = (world.values & ~states[currentStep].dontcare);
	bfield_t objState = states[currentStep].values;
	if (currentStep > 0)
		objState = states[currentStep].values & ~states[currentStep-1].values;
	else
		objState = states[currentStep].values & ~stateZero.values;
	bool fulfilled = ((worldState & objState) == objState);
	if (fulfilled) {
		if (currentStep > 0)
			objState = states[currentStep].values | ~states[currentStep - 1].values;
		else
			objState = states[currentStep].values | ~stateZero.values;
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


// GETTER ====================================
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


// EXTRA =================================

void NPC::setupCrewArea() {
	addTrackableRoom("Mezzanine");
	addTrackableRoom("CrewCorridor");
	goap_set_pre(&ap, "enter_CrewArea", "in_Mezzanine", true);
	goap_set_pst(&ap, "enter_CrewArea", "in_CrewArea", true);
	goap_set_pre(&ap, "leave_CrewArea", "in_CrewCorridor", true);
	goap_set_pst(&ap, "leave_CrewArea", "in_CrewArea", false);
}
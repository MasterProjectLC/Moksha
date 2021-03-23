#pragma once
#include "../npc.h"

using namespace std;

class Magnus : public NPC {
private:

public:
	Magnus() : NPC{ NULL, "Magnus", "", M, 0, 0 } {}
	explicit Magnus(Map* m);
	int decideActionParticular(string action) override;
	void advancePlansExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
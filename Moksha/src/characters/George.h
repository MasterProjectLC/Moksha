#pragma once
#include "../npc.h"

using namespace std;

class George : public NPC {
private:

public:
	George() : NPC{ NULL, "George", "", M, 0, 0 } {}
	explicit George(Map* m);
	int decideActionParticular(string action) override;
	void advancePlansExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
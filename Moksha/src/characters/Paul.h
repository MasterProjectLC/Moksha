#pragma once
#include "../npc.h"

using namespace std;

class Paul : public NPC {
private:

public:
	Paul() : NPC{ NULL, "Paul", "", M, 0, 0 } {}
	explicit Paul(Map* m);
	int decideActionParticular(string action) override;
	void updateProcessExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
#pragma once
#include "../npc.h"

using namespace std;

class Paul : public NPC {
private:

public:
	Paul() : NPC(NULL, "") {}
	explicit Paul(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
#pragma once
#include "../npc.h"

using namespace std;

class George : public NPC {
private:

public:
	George() : NPC(NULL, "") {}
	explicit George(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
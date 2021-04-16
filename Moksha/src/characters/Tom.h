#pragma once
#include "../npc.h"

using namespace std;

class Tom : public NPC {
private:

public:
	Tom() : NPC(NULL, "") {}
	explicit Tom(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
#pragma once
#include "../npc.h"

using namespace std;

class Baxter : public NPC {
private:

public:
	Baxter() : NPC{ NULL, "Baxter", "", M, 0, 0 } {}
	explicit Baxter(Map* m);
	int decideActionParticular(string action) override;
	void updateProcessExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
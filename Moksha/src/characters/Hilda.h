#pragma once
#include "../npc.h"

using namespace std;

class Hilda : public NPC {
private:

public:
	Hilda() : NPC(NULL, "") {}
	explicit Hilda(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
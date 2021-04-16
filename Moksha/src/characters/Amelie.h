#pragma once
#include "../npc.h"

using namespace std;

class Amelie : public NPC {
private:

public:
	Amelie() : NPC(NULL, "") {}
	explicit Amelie(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
#pragma once
#include "../npc.h"

using namespace std;

class Renard : public NPC {
private:
	int photos_taken;
public:
	Renard() : NPC(NULL, "") {}
	explicit Renard(Map* m);
	int decideActionParticular(string action) override;
	void setupProcessParticular(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};
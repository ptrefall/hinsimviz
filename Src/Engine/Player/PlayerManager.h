#pragma once

#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Player
{
class IPlayer;
class PlayerManager
{
public:
	PlayerManager(Core::CoreManager *coreMgr);
	~PlayerManager();

	int init();

	unsigned int genPlayerId() { playerIds++; return playerIds; }

	IPlayer *getHuman() const { return human; }
	IPlayer *getAI(unsigned int i);
	unsigned int getAICount() { return ai.size(); }

private:
	Core::CoreManager *coreMgr;

	IPlayer *human;
	std::vector<IPlayer*> ai;

	unsigned int playerIds;
};

}
}

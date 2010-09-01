#pragma once

#include <ClanLib/core.h>
#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace GameState
{
class IGameState;
class GameStateFactory;

class GameStateManager
{
public:
	GameStateManager(Core::CoreManager *coreMgr);
	virtual ~GameStateManager();

	virtual int init();

	virtual unsigned int genGameStateId();
	virtual IGameState *getGameState(unsigned int id) const;
	virtual IGameState *getCurrentState() const;

	virtual int changeState(unsigned int id);
	virtual int pushState(unsigned int id);
	virtual int popState();

	virtual IGameState *create(const CL_String &name);

private:
	Core::CoreManager *coreMgr;

	GameStateFactory *factory;

	unsigned int gameStateIds;
	std::vector<IGameState*> gameStates;
	IGameState *currentState;

	std::vector<IGameState*> gameStateStackHistory;
};

}
}

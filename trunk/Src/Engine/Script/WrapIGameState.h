#pragma once

#include <Lua/LuaPlus.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace GameState { class IGameState; }
namespace Script
{
class WrapGameStateManager;

class WrapIGameState
{
public:
	WrapIGameState(Core::CoreManager *coreMgr, WrapGameStateManager *wGameStateMgr, GameState::IGameState *gameState);
	~WrapIGameState();

	int init();

	GameState::IGameState *getGameState() const { return gameState; }
	LuaPlus::LuaObject getLGameState() const { return lGameState; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

private:
	Core::CoreManager *coreMgr;
	Script::WrapGameStateManager *wGameStateMgr;

	GameState::IGameState *gameState;
	LuaPlus::LuaObject lGameState;
	LuaPlus::LuaObject lMeta;
};

}
}

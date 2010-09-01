#pragma once

#include <Lua/LuaPlus.h>
#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{
class WrapIGameState;

class WrapGameStateManager
{
public:
	WrapGameStateManager(Core::CoreManager *coreMgr);
	~WrapGameStateManager();

	int init();

	LuaPlus::LuaObject &getLGameStates() { return lGameStates; }
	LuaPlus::LuaObject getLGameState(unsigned int id);
	WrapIGameState *getWGameState(unsigned int id);

private:
	void ChangeGameState(LuaPlus::LuaObject lGameState);

	Core::CoreManager *coreMgr;

	LuaPlus::LuaObject lGameStates;
	std::vector<WrapIGameState*> wGameStates;
};

}
}

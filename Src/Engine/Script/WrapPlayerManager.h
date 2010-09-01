#pragma once

#include <Lua/LuaPlus.h>
#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{
class WrapIPlayer;

class WrapPlayerManager
{
public:
	WrapPlayerManager(Core::CoreManager *coreMgr);
	~WrapPlayerManager();

	int init();

	LuaPlus::LuaObject &getLPlayers() { return lPlayers; }
	LuaPlus::LuaObject getLPlayer(unsigned int id);
	WrapIPlayer *getWPlayer(unsigned int id);

	void setLHuman(LuaPlus::LuaObject lHuman) { this->lHuman = lHuman; }
	void setWHuman(WrapIPlayer *wHuman) { this->wHuman = wHuman; }

	LuaPlus::LuaObject getLHuman() { return lHuman; }
	WrapIPlayer *getWHuman() { return wHuman; }

private:
	Core::CoreManager *coreMgr;

	LuaPlus::LuaObject lPlayers;
	std::vector<WrapIPlayer*> wPlayers;
	
	LuaPlus::LuaObject lHuman;
	WrapIPlayer *wHuman;
};

}
}

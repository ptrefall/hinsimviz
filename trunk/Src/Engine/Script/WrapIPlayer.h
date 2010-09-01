#pragma once

#include <Lua/LuaPlus.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Player { class IPlayer; }
namespace Script
{
class WrapPlayerManager;

class WrapIPlayer
{
public:
	WrapIPlayer(Core::CoreManager *coreMgr, WrapPlayerManager *wPlayerMgr, Player::IPlayer *player, bool isHuman = false);
	~WrapIPlayer();

	int init();

	Player::IPlayer *getPlayer() const { return player; }
	LuaPlus::LuaObject getLPlayer() const { return lPlayer; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

private:
	void SendCommand(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lCommand);
	void SendEvent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lEventType, LuaPlus::LuaObject lEventArg);
	void AddObject(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lObj);
	LuaPlus::LuaObject GetRelativeFocus(LuaPlus::LuaObject lSelf);

	Core::CoreManager *coreMgr;
	Script::WrapPlayerManager *wPlayerMgr;

	Player::IPlayer *player;
	LuaPlus::LuaObject lPlayer;
	LuaPlus::LuaObject lMeta;

	bool isHuman;
};

}
}

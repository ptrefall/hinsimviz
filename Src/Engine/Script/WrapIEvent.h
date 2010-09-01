#pragma once

#include <Lua/LuaPlus.h>
#include <Clanlib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Events { class IEvent; }
namespace Script
{

class WrapIEvent
{
public:
	WrapIEvent(Core::CoreManager *coreMgr, const Events::IEvent *event);
	~WrapIEvent();

	int init();

	LuaPlus::LuaObject getLEvent() const { return lEvent; }
	LuaPlus::LuaObject getLEvents() const { return lEvents; }

private:
	Core::CoreManager *coreMgr;

	const Events::IEvent *event;
	LuaPlus::LuaObject lEvent;
	LuaPlus::LuaObject lEvents;
};

}
}

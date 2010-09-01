#pragma once

#include <Lua/LuaPlus.h>
#include <Clanlib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Entity { class Component; }
namespace Script
{
class WrapObject;
class WrapComponentContainer;

class WrapComponent
{
public:
	WrapComponent(Core::CoreManager *coreMgr, Script::WrapObject *wObj, WrapComponentContainer *wCompContainer, Entity::Component *component);
	~WrapComponent();

	int init();

	Entity::Component *getComp() const { return component; }
	LuaPlus::LuaObject getLComp() const { return lComponent; }
	WrapObject *getWObj() const { return wObj; }

private:
	Core::CoreManager *coreMgr;
	Script::WrapComponentContainer *wCompContainer;
	Script::WrapObject *wObj;

	Entity::Component *component;
	LuaPlus::LuaObject lComponent;
};

}
}

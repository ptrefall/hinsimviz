#pragma once

#include <Lua/LuaPlus.h>
#include <vector>
#include <Engine/Events/EngineEventContainer.h>
#include <Engine/Events/EngineEvent.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{
class WrapObject;
class WrapComponent;

class WrapComponentContainer
{
public:
	WrapComponentContainer(Core::CoreManager *coreMgr, Script::WrapObject *wObj);
	~WrapComponentContainer();

	int init();

	LuaPlus::LuaObject &getLComps() { return lComponents; }

private:
	void AddComponent(LuaPlus::LuaObject self, LuaPlus::LuaObject lName);

	Core::CoreManager *coreMgr;
	Script::WrapObject *wObj;

	LuaPlus::LuaObject lComponents;
	std::vector<WrapComponent*> wComponents;

	void OnComponentAdded(const Engine::Events::EngineEvent &event);
	Engine::Events::EngineEventContainer engineEvents;
};

}
}

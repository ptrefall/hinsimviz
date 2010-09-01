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

class WrapObjectManager
{
public:
	WrapObjectManager(Core::CoreManager *coreMgr);
	~WrapObjectManager();

	int init();

	LuaPlus::LuaObject &getLObjs() { return lObjs; }
	LuaPlus::LuaObject getLObj(unsigned int id);
	WrapObject *getWObj(unsigned int id);

private:
	LuaPlus::LuaObject CreateObject(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName);

	Core::CoreManager *coreMgr;

	LuaPlus::LuaObject lObjs;
	std::vector<WrapObject*> wObjs;

	void OnObjectCreated(const Engine::Events::EngineEvent &event);
	void OnObjectRemoved(const Engine::Events::EngineEvent &event);
	Engine::Events::EngineEventContainer engineEvents;
};

}
}

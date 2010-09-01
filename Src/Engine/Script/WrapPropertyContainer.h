#pragma once

#include <Lua/LuaPlus.h>
#include <vector>
#include <ClanLib/core.h>
#include <Depends/Entity/Property.h>
#include <Engine/Events/EngineEventContainer.h>
#include <Engine/Events/EngineEvent.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Node; }
namespace Script
{
class WrapObject;
class WrapNode;
class WrapIProperty;

class WrapPropertyContainer
{
public:
	WrapPropertyContainer(Core::CoreManager *coreMgr, Script::WrapObject *wObj);
	WrapPropertyContainer(Core::CoreManager *coreMgr, Script::WrapNode *wNode);
	~WrapPropertyContainer();

	int init();

	LuaPlus::LuaObject &getLProps() { return lProperties; }

private:
	void AddProperty(LuaPlus::LuaObject self, LuaPlus::LuaObject name, LuaPlus::LuaObject defValue);

	void add(const CL_String &name, Entity::IProperty *prop, WrapIProperty *wProp, bool isObj);

	Core::CoreManager *coreMgr;
	Script::WrapObject *wObj;
	Script::WrapNode *wNode;

	LuaPlus::LuaObject lProperties;
	std::vector<WrapIProperty*> wProperties;

	void OnPropertyAdded(const Engine::Events::EngineEvent &event);
	Engine::Events::EngineEventContainer engineEvents;
};

}
}

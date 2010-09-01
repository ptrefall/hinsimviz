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
class WrapNode;

class WrapSceneManager
{
public:
	WrapSceneManager(Core::CoreManager *coreMgr);
	~WrapSceneManager();

	int init();

	LuaPlus::LuaObject &getLNodes() { return lNodes; }
	LuaPlus::LuaObject getLNode(unsigned int id);
	WrapNode *getWNode(unsigned int id);

	void addWNode(WrapNode *wNode);
	void removeWNode(WrapNode *wNode);

private:
	LuaPlus::LuaObject CreateNode(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName);
	void ActivateCamera(LuaPlus::LuaObject lCamera);

	Core::CoreManager *coreMgr;

	LuaPlus::LuaObject lNodes;
	std::vector<WrapNode*> wNodes;

	void OnNodeCreated(const Engine::Events::EngineEvent &event);
	void OnNodeRemoved(const Engine::Events::EngineEvent &event);
	Engine::Events::EngineEventContainer engineEvents;
};

}
}

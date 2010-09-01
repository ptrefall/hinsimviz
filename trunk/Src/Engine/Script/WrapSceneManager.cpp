#include "WrapSceneManager.h"
#include "WrapNode.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/IEngineEventManager.h>
#include "ScriptManager.h"
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Node.h>
#include <Engine/Scene/Camera.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapSceneManager::WrapSceneManager(Core::CoreManager *coreMgr)
: engineEvents(coreMgr->getEngineEventMgr())
{
	this->coreMgr = coreMgr;
}

WrapSceneManager::~WrapSceneManager()
{
}

int WrapSceneManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("CreateNode", *this, &WrapSceneManager::CreateNode);
	globals.RegisterDirect("ActivateCamera", *this, &WrapSceneManager::ActivateCamera);
	lNodes = globals.CreateTable("Nodes");

	engineEvents.Connect("NodeCreated", this, &WrapSceneManager::OnNodeCreated);
	engineEvents.Connect("NodeRemoved", this, &WrapSceneManager::OnNodeRemoved);
	return 0;
}

LuaObject WrapSceneManager::getLNode(unsigned int id)
{
	return lNodes.GetByIndex((int)id);
}

WrapNode *WrapSceneManager::getWNode(unsigned int id)
{
	for(unsigned int i = 0; i < wNodes.size(); i++)
	{
		if(wNodes[i]->getNode()->getId() == id)
		{
			return wNodes[i];
		}
	}
	return NULL;
}

void WrapSceneManager::addWNode(WrapNode *wNode)
{
	for(unsigned int i = 0; i < wNodes.size(); i++)
	{
		if(wNode == wNodes[i])
			return;
	}
	wNodes.push_back(wNode);
}

void WrapSceneManager::removeWNode(WrapNode *wNode)
{
	for(unsigned int i = 0; i < wNodes.size(); i++)
	{
		if(wNode == wNodes[i])
		{
			wNodes.erase(wNodes.begin() + i);
			return;
		}
	}
}

LuaObject WrapSceneManager::CreateNode(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName)
{
	if(!lType.IsString())
	{
		CL_String type_type = lType.TypeName();
		
		CL_String msg = cl_format("Failed to create node, because the type of type was %1 when expecting String", type_type);
		coreMgr->getLogMgr()->log("WrapSceneManager::CreateNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	if(!lName.IsString() && !lName.IsNil())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String msg = cl_format("Failed to create node, because the type of name was %1 when expecting String", name_type);
		coreMgr->getLogMgr()->log("WrapSceneManager::CreateNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	CL_String type = lType.ToString();
	CL_String name;

	Engine::Scene::Node *node = NULL;
	if(lName.IsString())
	{
		name = lName.ToString();
		node = coreMgr->getSceneMgr()->create(type, name);
	}
	else
	{
		node = coreMgr->getSceneMgr()->create(type);
	}
	
	if(node == NULL)
	{
		CL_String msg = cl_format("Failed to create node %1 of type %2", name, type);
		coreMgr->getLogMgr()->log("WrapSceneManager::CreateNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	coreMgr->getSceneMgr()->getRoot()->add(node);

	WrapNode *wNode = getWNode(node->getId());
	if(wNode)
		return wNode->getLNode();
	else
		return LuaObject();
}

void WrapSceneManager::ActivateCamera(LuaPlus::LuaObject lCamera)
{
	if(!lCamera.IsTable())
	{
		CL_String camera_type = lCamera.TypeName();
		
		CL_String msg = cl_format("Failed to set active camera, because the type of camera was %1 when expecting table", camera_type);
		coreMgr->getLogMgr()->log("WrapSceneManager::ActiveCamera", msg, Log::L_ERROR);
		return;
	}

	unsigned int camId = (unsigned int)lCamera.GetByName("id").ToInteger();
	for(unsigned int i = 0; i < wNodes.size(); i++)
	{
		if(wNodes[i]->getNode()->getId() == camId)
		{
			Engine::Scene::Camera *camera = dynamic_cast<Engine::Scene::Camera*>(wNodes[i]->getNode()->findNode("Camera"));
			if(camera)
				coreMgr->getSceneMgr()->setActiveCamera(camera);
			break;
		}
	}
}

void WrapSceneManager::OnNodeCreated(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Node *node = event.getArgument(0).ToNode();

	WrapNode *wNode = new WrapNode(coreMgr, this, node);
	int fail = wNode->init();
	if(fail)
	{
		delete wNode;
		wNode = NULL;
		
		CL_String msg = cl_format("Failed to initialize node wrapper for type %1", node->getName());
		coreMgr->getLogMgr()->log("WrapSceneManager::OnNodeCreated", msg, Log::L_ERROR);
		return;
	}
	wNodes.push_back(wNode);
}

void WrapSceneManager::OnNodeRemoved(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Node *node = event.getArgument(0).ToNode();
	for(unsigned int i = 0; i < wNodes.size(); i++)
	{
		if(wNodes[i]->getNode()->getId() == node->getId())
		{
			WrapNode *wNode = wNodes[i];
			wNodes.erase(wNodes.begin() + i);
			delete wNode;
			wNode = NULL;
			break;
		}
	}
}

#include "WrapObjectManager.h"
#include "WrapObject.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/IEngineEventManager.h>
#include <Engine/Scene/SceneManager.h>
#include "ScriptManager.h"
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapObjectManager::WrapObjectManager(Core::CoreManager *coreMgr)
: engineEvents(coreMgr->getEngineEventMgr())
{
	this->coreMgr = coreMgr;
}

WrapObjectManager::~WrapObjectManager()
{
}

int WrapObjectManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("CreateObject", *this, &WrapObjectManager::CreateObject);
	lObjs = globals.CreateTable("Objects");

	engineEvents.Connect("ObjectCreated", this, &WrapObjectManager::OnObjectCreated);
	engineEvents.Connect("ObjectRemoved", this, &WrapObjectManager::OnObjectRemoved);
	return 0;
}

LuaObject WrapObjectManager::getLObj(unsigned int id)
{
	return lObjs.GetByIndex((int)id);
}

WrapObject *WrapObjectManager::getWObj(unsigned int id)
{
	for(unsigned int i = 0; i < wObjs.size(); i++)
	{
		if(wObjs[i]->getObj()->getId() == id)
		{
			return wObjs[i];
		}
	}
	return NULL;
}

LuaObject WrapObjectManager::CreateObject(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName)
{
	if(!lType.IsString())
	{
		CL_String type_type = lType.TypeName();
		
		CL_String msg = cl_format("Failed to create object, because the type of type was %1 when expecting String", type_type);
		coreMgr->getLogMgr()->log("WrapObjectManager::CreateObject", msg, Log::L_ERROR);
		return LuaObject();
	}
	if(!lName.IsString() && !lName.IsNil())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String msg = cl_format("Failed to create object, because the type of name was %1 when expecting String or Nil", name_type);
		coreMgr->getLogMgr()->log("WrapObjectManager::CreateObject", msg, Log::L_ERROR);
		return LuaObject();
	}

	CL_String type = lType.ToString();
	CL_String name;

	Engine::Scene::Object *obj = NULL;
	if(lName.IsString())
	{
		name = lName.ToString();
		if(name != CL_String())
		{
			obj = coreMgr->getObjectMgr()->create(type, name);
		}
		else
		{
			obj = coreMgr->getObjectMgr()->create(type);
		}
	}
	else
	{
		obj = coreMgr->getObjectMgr()->create(type);
	}

	if(obj == NULL)
	{
		CL_String msg = cl_format("Failed to create object %1 of type %2", name, type);
		coreMgr->getLogMgr()->log("WrapObjectManager::CreateObject", msg, Log::L_ERROR);
		return LuaObject();
	}

	if(coreMgr->getSceneMgr()->getRoot())
	{
		coreMgr->getSceneMgr()->getRoot()->add(obj);
	}
	else
	{
		CL_String msg = cl_format("Could not get root from scene, it's NULL!");
		coreMgr->getLogMgr()->log("WrapObjectManager::CreateObject", msg, Log::L_WARN);
	}

	WrapObject *wObj = getWObj(obj->getId());

	if(wObj)
		return wObj->getLObj();
	else
		return LuaObject();
}

void WrapObjectManager::OnObjectCreated(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Object *obj = event.getArgument(0).ToObject();
	obj->init();

	WrapObject *wObj = new WrapObject(coreMgr, this, obj);
	int fail = wObj->init();
	if(fail)
	{
		delete wObj;
		wObj = NULL;
		
		CL_String msg = cl_format("Failed to initialize object wrapper for type %1", obj->getName());
		coreMgr->getLogMgr()->log("WrapObjectManager::OnObjectCreated", msg, Log::L_ERROR);
		return;
	}
	wObjs.push_back(wObj);
}

void WrapObjectManager::OnObjectRemoved(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Object *obj = event.getArgument(0).ToObject();
	for(unsigned int i = 0; i < wObjs.size(); i++)
	{
		if(wObjs[i]->getObj()->getId() == obj->getId())
		{
			WrapObject *wObj = wObjs[i];
			wObjs.erase(wObjs.begin() + i);
			delete wObj;
			wObj = NULL;
			break;
		}
	}
}

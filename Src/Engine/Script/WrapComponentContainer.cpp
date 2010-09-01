#include "WrapComponentContainer.h"
#include "WrapComponent.h"
#include "WrapObject.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/IEngineEventManager.h>
#include <Engine/Scene/Object.h>
#include <Engine/Events/Event.h>
#include <Depends/Entity/Component.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapComponentContainer::WrapComponentContainer(Core::CoreManager *coreMgr, Script::WrapObject *wObj)
: engineEvents(coreMgr->getEngineEventMgr())
{
	this->coreMgr = coreMgr;
	this->wObj = wObj;
}

WrapComponentContainer::~WrapComponentContainer()
{
	for(unsigned int i = 0; i < wComponents.size(); i++)
	{
		WrapComponent *wComp = wComponents[i];
		delete wComp;
		wComp = NULL;
	}
	wComponents.clear();
	lComponents.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapComponentContainer::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	if(wObj)
	{
		lComponents = wObj->getLObj().CreateTable("Components");

		//obj.Components:Add("SomeComponent")
		{
			LuaObject lMeta = lComponents.CreateTable("MetaTable");
			lMeta.SetObject("__index", lMeta);
			lMeta.RegisterDirect("Add", *this, &WrapComponentContainer::AddComponent);
			lComponents.SetLightUserData("__object", this);
			lComponents.SetMetaTable(lMeta);
		}

		//obj:AddComponent("SomeComponent")
		{
			LuaObject lMeta = wObj->getLMeta();
			lMeta.RegisterDirect("AddComponent", *this, &WrapComponentContainer::AddComponent);
		}
		
		Engine::Scene::Object *obj = wObj->getObj();
		std::vector<Engine::Entity::Component*> &components = obj->GetComponents();
		std::vector<Engine::Entity::Component*>::iterator compIt = components.begin();
		for(; compIt != components.end(); ++compIt)
		{
			WrapComponent *wComp = new WrapComponent(coreMgr, wObj, this, (*compIt));
			int fail = wComp->init();
			if(fail)
			{
				delete wComp;
				wComp = NULL;

				CL_String err = cl_format("Failed to initialize component wrapper for Component %1", (*compIt)->GetName());
				coreMgr->getLogMgr()->log("WrapComponentContainer:Init", err, Log::L_ERROR);
				continue;
			}
			wComponents.push_back(wComp);
		}

		engineEvents.Connect("ComponentAdded", this, &WrapComponentContainer::OnComponentAdded);
	}
	else
		return 1;
	return 0;
}

void WrapComponentContainer::AddComponent(LuaObject self, LuaObject lName)
{
	if(!self.IsTable())
	{
		CL_String self_type = self.TypeName();
		
		CL_String err = cl_format("Failed to add component, because the type of self was %1 when expecting Table", self_type);
		coreMgr->getLogMgr()->log("WrapComponentContainer:AddComponent", err, Log::L_ERROR);
		return;
	}

	if(!lName.IsString())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String err = cl_format("Failed to add component, because the type of name was %1 when expecting String", name_type);
		coreMgr->getLogMgr()->log("WrapComponentContainer:AddComponent", err, Log::L_ERROR);
		return;
	}

	CL_String name = lName.ToString();
	
	if(wObj == NULL)
	{
		CL_String err = cl_format("Failed to add component %1, because there is no object!", name);
		coreMgr->getLogMgr()->log("WrapComponentContainer:AddComponent", err, Log::L_ERROR);
		return;
	}

	Entity::Component *comp = wObj->getObj()->AddComponent(name);
	if(comp == NULL)
	{
		CL_String err = cl_format("Failed to add component %1, because no component was returned!", name);
		coreMgr->getLogMgr()->log("WrapComponentContainer:AddComponent", err, Log::L_ERROR);
		return;
	}

	return;
}

void WrapComponentContainer::OnComponentAdded(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Object *go = event.getArgument(1).ToObject();
	if(go->getId() != wObj->getObj()->getId())
		return;

	Engine::Entity::Component *comp = event.getArgument(0).ToComponent();

	WrapComponent *wComp = new WrapComponent(coreMgr, wObj, this, comp);
	int fail = wComp->init();
	if(fail)
	{
		delete wComp;
		wComp = NULL;
		
		CL_String err = cl_format("Failed to initialize component wrapper for Component %1", comp->GetName());
		coreMgr->getLogMgr()->log("WrapComponentContainer:OnComponentAdded", err, Log::L_ERROR);
		return;
	}
	wComponents.push_back(wComp);
}

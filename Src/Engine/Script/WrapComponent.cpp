#include "WrapComponent.h"
#include "WrapObject.h"
#include "WrapComponentContainer.h"
#include "LuaComponent.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Depends/Entity/Component.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>
#include <Engine/Events/Event.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;
using namespace Entity;

WrapComponent::WrapComponent(Core::CoreManager *coreMgr, Script::WrapObject *wObj, WrapComponentContainer *wCompContainer, Entity::Component *component)
{
	this->coreMgr = coreMgr;
	this->wObj = wObj;
	this->wCompContainer = wCompContainer;
	this->component = component;
}

WrapComponent::~WrapComponent()
{
	lComponent.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapComponent::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	//Check if this is a C++ component, or a scripted Lua defined component
	Component::LuaComponent *luaComp = dynamic_cast<Component::LuaComponent*>(component);
	if(luaComp == NULL)
	{
		LuaObject &lComps = wCompContainer->getLComps();
		lComponent = lComps.CreateTable(component->GetName().c_str());
		lComponent.SetString("id", component->GetName().c_str());
		
		{
			LuaObject lMeta = lComponent.CreateTable("MetaTable");
			lMeta.SetObject("__index", lMeta);
			
			lComponent.SetLightUserData("__object", this);
			lComponent.SetMetaTable(lMeta);
		}
	}
	else
	{
		lComponent = globals.GetByName(component->GetName().c_str());
		lComponent.SetString("id", component->GetName().c_str());
		{
			LuaObject lMeta = lComponent.CreateTable("MetaTable");
			lMeta.SetObject("__index", lMeta);
			
			lComponent.SetLightUserData("__object", this);
			lComponent.SetMetaTable(lMeta);
		}

		luaComp->initLuaExposure(this);
	}
	return 0;
}

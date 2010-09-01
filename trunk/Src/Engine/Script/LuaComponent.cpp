#include "LuaComponent.h"
#include "WrapComponent.h"
#include "WrapObjectManager.h"
#include "WrapIEvent.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>
#include <Depends/Entity/ComponentContainer.h>
#include <Depends/Entity/PropertyContainer.h>
#include <Engine/Events/Event.h>
#include <Engine/Events/CommandHandler.h>
#include <Engine/Player/IPlayer.h>

#include <Lua/LuaPlus.h>

using namespace Engine;
using namespace Component;
using namespace LuaPlus;

LuaComponent::LuaComponent(Engine::Core::CoreManager *coreMgr, Engine::Scene::Object *go, const CL_String &name)
: Engine::Entity::Component(static_cast<Engine::Entity::ComponentContainer*>(go), static_cast<Engine::Entity::PropertyContainer*>(go), name), coreMgr(coreMgr), go(go)
{
	wComp = NULL;
	hasInit = false;
	hasUpdate = false;
	hasCommand = false;
	hasEvent = false;
}

void LuaComponent::initLuaExposure(Script::WrapComponent *wComp)
{
	this->wComp = wComp;
	
	hasInit = false;
	LuaObject lInit = wComp->getLComp().GetByName("OnInit");
	if(lInit.IsFunction())
	{
		hasInit = true;
	}

	hasUpdate = false;
	LuaObject lUpdate = wComp->getLComp().GetByName("OnUpdate");
	if(lUpdate.IsFunction())
	{
		hasUpdate = true;
	}
	
	hasCommand = false;
	LuaObject lCommand = wComp->getLComp().GetByName("OnCommand");
	if(lCommand.IsFunction())
	{
		hasCommand = true;
	}
	
	hasEvent = false;
	LuaObject lEvent = wComp->getLComp().GetByName("OnEvent");
	if(lEvent.IsFunction())
	{
		hasEvent = true;
	}

	if(hasInit)
	{
		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("LuaComponent:InitLuaExposure", err, Log::L_ERROR);
			return;
		}

		coreMgr->getScriptMgr()->doString(cl_format("%1:OnInit(Objects[%2])", name, go->getId()));
	}
}

void LuaComponent::ExecuteCommand(const CL_String &command, Engine::Player::IPlayer *player)
{
	if(hasCommand)
	{
		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("LuaComponent:ExecuteCommand", err, Log::L_ERROR);
			return;
		}

		if(player)
			coreMgr->getScriptMgr()->doString(cl_format("%1:OnCommand(Objects[%2], '%3', %4)", name, go->getId(), command, player->getId()));
		else
			coreMgr->getScriptMgr()->doString(cl_format("%1:OnCommand(Objects[%2], '%3', nil)", name, go->getId(), command));
	}
}

void LuaComponent::ExecuteEvent(const Engine::Events::IEvent &event, Engine::Player::IPlayer *player)
{
	if(hasEvent)
	{
		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("LuaComponent:ExecuteEvent", err, Log::L_ERROR);
			return;
		}

		Script::WrapIEvent wEvent(coreMgr, &event);
		int fail = wEvent.init();
		if(fail)
			return;

		//Make sure that the event exist in the global state's Events table
		LuaObject lEvent = wEvent.getLEvents().GetByName(event.getType());
		if(lEvent.IsNil())
		{
			CL_String err = cl_format("Event of type %1 has not been exposed to Lua!", event.getType());
			coreMgr->getLogMgr()->log("LuaComponent:ExecuteEvent", err, Log::L_ERROR);
			return;
		}

		if(player)
			coreMgr->getScriptMgr()->doString(cl_format("%1:OnEvent(Objects[%2], Events['%3'], %4)", name, go->getId(), event.getType(), player->getId()));
		else
			coreMgr->getScriptMgr()->doString(cl_format("%1:OnEvent(Objects[%2], Events['%3'], nil)", name, go->getId(), event.getType()));
	}
}

void LuaComponent::Update(double dt)
{
	if(hasUpdate)
	{
		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("LuaComponent:Update", err, Log::L_ERROR);
			return;
		}

		coreMgr->getScriptMgr()->doString(cl_format("%1:OnUpdate(Objects[%2], %3)", name, go->getId(), dt));
	}
}

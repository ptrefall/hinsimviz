#include "WrapIEvent.h"
#include "WrapObjectManager.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/IEvent.h>
#include <Engine/Events/Event.h>
#include <Engine/Scene/Object.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;
using namespace Events;

WrapIEvent::WrapIEvent(Core::CoreManager *coreMgr, const Events::IEvent *event)
: event(event)
{
	this->coreMgr = coreMgr;
}

WrapIEvent::~WrapIEvent()
{
	lEvent.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapIEvent::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	if(lEvents.IsNil())
		lEvents = globals.GetByName("Events");
	if(lEvents.IsNil())
		lEvents = globals.CreateTable("Events");

	lEvent = lEvents.CreateTable(event->getType());
	lEvent.SetString("id", event->getType());
	if(event->getArgType() == Engine::Events::TYPE_BOOL)
	{
		const Engine::Events::Event<bool, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<bool, Engine::Scene::Object*>*>(event);

		bool arg = e->getArg();
		lEvent.SetBoolean("arg", arg);

		Engine::Scene::Object *go = e->getSelf();

		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("self", lObj);
	}
	else if(event->getArgType() == Engine::Events::TYPE_INT)
	{
		if(event->getSelfType() == Engine::Events::TYPE_OBJECT)
		{
			const Engine::Events::Event<int, Engine::Scene::Object*> *e;
			e = static_cast<const Engine::Events::Event<int, Engine::Scene::Object*>*>(event);

			int arg = e->getArg();
			lEvent.SetInteger("arg", arg);

			Engine::Scene::Object *go = e->getSelf();

			//Make sure that the object exist in the global state's Objects table
			LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
			if(lObj.IsNil())
			{
				CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
				coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
				return 1;
			}
			lEvent.SetObject("self", lObj);
		}
		else if(event->getSelfType() == Engine::Events::TYPE_INT)
		{
			const Engine::Events::Event<int,int> *e;
			e = static_cast<const Engine::Events::Event<int, int>*>(event);

			int arg = e->getArg();
			lEvent.SetInteger("arg", arg);

			int self = e->getSelf();
			lEvent.SetInteger("self", self);
		}
	}
	else if(event->getArgType() == Engine::Events::TYPE_UINT)
	{
		const Engine::Events::Event<unsigned int, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<unsigned int, Engine::Scene::Object*>*>(event);

		unsigned int arg = e->getArg();
		lEvent.SetInteger("arg", arg);

		Engine::Scene::Object *go = e->getSelf();

		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("self", lObj);
	}
	else if(event->getArgType() == Engine::Events::TYPE_FLOAT)
	{
		const Engine::Events::Event<float, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<float, Engine::Scene::Object*>*>(event);

		float arg = e->getArg();
		lEvent.SetNumber("arg", arg);

		Engine::Scene::Object *go = e->getSelf();

		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("self", lObj);
	}
	else if(event->getArgType() == Engine::Events::TYPE_DOUBLE)
	{
		const Engine::Events::Event<double, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<double, Engine::Scene::Object*>*>(event);

		double arg = e->getArg();
		lEvent.SetNumber("arg", arg);

		Engine::Scene::Object *go = e->getSelf();

		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("self", lObj);
	}
	else if(event->getArgType() == Engine::Events::TYPE_STRING)
	{
		const Engine::Events::Event<CL_String, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<CL_String, Engine::Scene::Object*>*>(event);

		CL_String arg = e->getArg();
		lEvent.SetString("arg", arg.c_str());

		Engine::Scene::Object *go = e->getSelf();
		LuaObject lObj;
		if(go == NULL)
		{
			lObj.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
			lEvent.SetObject("self", lObj);
		}
		else
		{
			//Make sure that the object exist in the global state's Objects table
			lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
			if(lObj.IsNil())
			{
				CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
				coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
				return 1;
			}
			lEvent.SetObject("self", lObj);
		}
	}
	else if(event->getArgType() == Engine::Events::TYPE_OBJECT)
	{
		const Engine::Events::Event<Engine::Scene::Object*, Engine::Scene::Object*> *e;
		e = static_cast<const Engine::Events::Event<Engine::Scene::Object*, Engine::Scene::Object*>*>(event);

		Engine::Scene::Object* arg = e->getArg();
		LuaObject lArg = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(arg->getId());
		if(lArg.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", arg->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("arg", lArg);

		Engine::Scene::Object *go = e->getSelf();

		//Make sure that the object exist in the global state's Objects table
		LuaObject lObj = coreMgr->getScriptMgr()->getWObjMgr()->getLObjs().GetByIndex(go->getId());
		if(lObj.IsNil())
		{
			CL_String err = cl_format("Object of id %1 has not been exposed to Lua!", go->getId());
			coreMgr->getLogMgr()->log("WrapIEvent::Init", err, Log::L_ERROR);
			return 1;
		}
		lEvent.SetObject("self", lObj);
	}
	
	{
		LuaObject lMeta = lEvent.CreateTable("MetaTable");
		lMeta.SetObject("__index", lMeta);
		
		lEvent.SetLightUserData("__object", this);
		lEvent.SetMetaTable(lMeta);
	}

	return 0;
}

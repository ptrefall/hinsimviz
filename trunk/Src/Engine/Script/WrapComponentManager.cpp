#include "WrapComponentManager.h"
#include "WrapComponent.h"
#include "LuaComponent.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>
#include <Depends/Entity/Component.h>
#include <Engine/Resource/ResManager.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapComponentManager::WrapComponentManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

WrapComponentManager::~WrapComponentManager()
{
}

int WrapComponentManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("RegisterComponent", *this, &WrapComponentManager::RegisterComponent);

	//Load all scripts in Game/Components
	coreMgr->getLogMgr()->log("WrapComponentManager:Init", "Loading scripted components!", Log::L_INFO);
	
	std::vector<CL_String> scripts = coreMgr->getResMgr()->getFilesInDir("/Script/Components/");
	for(unsigned int i = 0; i < scripts.size(); i++)
	{
		int fail = coreMgr->getScriptMgr()->doFile(cl_format("Components/%1", scripts[i]));
		if(fail)
		{
			CL_String err = cl_format("Failed to load component script %1", scripts[i]);
			coreMgr->getLogMgr()->log("WrapComponentManager:Init", err, Log::L_ERROR);
		}
	}
	coreMgr->getLogMgr()->log("WrapComponentManager:Init", "Finsihed loading scripted components!", Log::L_INFO);

	return 0;
}

void WrapComponentManager::RegisterComponent(LuaObject lName)
{
	if(!lName.IsString())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String err = cl_format("Failed to register component, because the type of name was %1 when expecting String!", name_type);
		coreMgr->getLogMgr()->log("WrapComponentManager:RegisterComponent", err, Log::L_ERROR);
		return;
	}

	CL_String name = lName.ToString();
	coreMgr->getObjectMgr()->getComponentFactory()->RegisterComponent(name, &Engine::Component::LuaComponent::Create);
	
	coreMgr->getLogMgr()->log("WrapComponentManager:RegisterComponent", cl_format("Registered scripted component %1", name), Log::L_DEBUG);
	return;
}

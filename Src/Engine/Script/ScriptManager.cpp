#include "ScriptManager.h"
#include "WrapLogManager.h"
#include "WrapObjectManager.h"
#include "WrapSceneManager.h"
#include "WrapPlayerManager.h"
#include "WrapGameStateManager.h"
#include "WrapCoreManager.h"
#include "WrapComponentManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

ScriptManager::ScriptManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

ScriptManager::~ScriptManager()
{
}

int ScriptManager::init()
{
	//Start LuaPlus here
	LuaObject globals = globalState->GetGlobals();
	metaTable = globals.CreateTable("ScriptManagerMetaTable");
	metaTable.SetObject("__index", metaTable);

	//metaTable.RegisterObjectDirect("DoFile", (ScriptManager *)0, &ScriptManager::doFile);
	//metaTable.RegisterObjectDirect("Print", (ScriptManager *)0, &ScriptManager::Print);

	LuaObject scriptManObj = globalState->BoxPointer(this);
	scriptManObj.SetMetaTable(metaTable);

	globalState->GetGlobals().SetObject("ScriptManager", scriptManObj);

	globals.RegisterDirect("Print", *this, &ScriptManager::Print);

	wrapLogMgr = new WrapLogManager(coreMgr);
	int fail = wrapLogMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Log Manager!", Log::L_ERROR);

	wrapObjMgr = new WrapObjectManager(coreMgr);
	fail = wrapObjMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Object Manager!", Log::L_ERROR);

	wrapSceneMgr = new WrapSceneManager(coreMgr);
	fail = wrapSceneMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Scene Manager!", Log::L_ERROR);

	wrapPlayerMgr = new WrapPlayerManager(coreMgr);
	fail = wrapPlayerMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Player Manager!", Log::L_ERROR);

	wrapGameStateMgr = new WrapGameStateManager(coreMgr);
	fail = wrapGameStateMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for GameState Manager!", Log::L_ERROR);

	wrapCoreMgr = new WrapCoreManager(coreMgr);
	fail = wrapCoreMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Core Manager!", Log::L_ERROR);

	wrapCompMgr = new WrapComponentManager(coreMgr);
	fail = wrapCompMgr->init();
	if(fail)
		coreMgr->getLogMgr()->log("ScriptManager::Init", "Failed to initialize the Script Wrapper for Component Manager!", Log::L_ERROR);

	return 0;
}

int ScriptManager::doFile(const CL_String &fileName)
{
	try
	{
		int value = globalState->DoFile(cl_format("%1Script/%2", coreMgr->getResMgr()->getRootPath(), fileName).c_str());
		if(value > 0)
		{
			CL_String err = cl_format("There was a problem executing the file %1, \nerror: %2", fileName, LuaStackObject(globalState.Get(), -1).GetString());
			coreMgr->getLogMgr()->log("ScriptManager::DoFile", err, Log::L_ERROR);
		}
		return value;
	}
	catch(const LuaPlus::LuaException &e)
	{
		CL_String err = cl_format("An error occured when trying to extract a file: %1 \n%2", e.GetErrorMessage(), LuaStackObject(globalState.Get(), -1).GetString());
		coreMgr->getLogMgr()->log("ScriptManager::DoFile", err, Log::L_ERROR);
		return 1;
	}
}

int ScriptManager::doString(const CL_String &script, bool throwException)
{
	if(throwException)
	{
		int value = globalState->DoString(script.c_str());
		return value;
	}
	else
	{
		try
		{
			int value = globalState->DoString(script.c_str());
			if(value > 0)
			{
				CL_String err = cl_format("There was a problem executing the string %1, \nerror: %2", script, LuaStackObject(globalState.Get(), -1).GetString());
				coreMgr->getLogMgr()->log("ScriptManager::DoString", err, Log::L_ERROR);
			}
			return value;
		}
		catch(const LuaPlus::LuaException &e)
		{
			CL_String err = cl_format("An error occured when trying to extract a script: %1 \n%2", e.GetErrorMessage(), LuaStackObject(globalState.Get(), -1).GetString());
			coreMgr->getLogMgr()->log("ScriptManager::DoString", err, Log::L_ERROR);
			return 1;
		}
	}
}

CL_String ScriptManager::getErrorOnStack()
{
	return CL_String(LuaStackObject(globalState.Get(), -1).GetString());
}

void ScriptManager::Print(LuaObject text)
{
	if(!text.IsString() && !text.IsBoolean() && !text.IsNumber())
	{
		CL_String type_name = text.TypeName();
		CL_String err = cl_format("%1%2", "ScriptError: The type of text is", type_name);
		coreMgr->getLogMgr()->log("ScriptManager::Print", err, Log::L_ERROR);
		return;
	}
	
	if(text.IsString())
	{
		CL_String value = text.ToString();
		std::cout << "**** Script: " << value.c_str() << std::endl;
	}
	else if(text.IsBoolean())
	{
		bool value = text.GetBoolean();
		if(value)
			std::cout << "**** Script: " << "true" << std::endl;
		else
			std::cout << "**** Script: " << "false" << std::endl;
	}
	else if(text.IsNumber())
	{
		float value = text.GetFloat();
		std::cout << "**** Script: " << value << std::endl;
	}
}

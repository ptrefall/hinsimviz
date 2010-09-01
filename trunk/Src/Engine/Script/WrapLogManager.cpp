#include "WrapLogManager.h"
#include <Engine/Core/CoreManager.h>
#include "ScriptManager.h"
#include <Log/LogManager.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapLogManager::WrapLogManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

WrapLogManager::~WrapLogManager()
{
}

int WrapLogManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("Log", *this, &WrapLogManager::Log);
	globals.SetInteger("LOG_INFO", Log::L_INFO);
	globals.SetInteger("LOG_DEBUG", Log::L_DEBUG);
	globals.SetInteger("LOG_WARN", Log::L_WARN);
	globals.SetInteger("LOG_ERROR", Log::L_ERROR);
	globals.SetInteger("LOG_FATAL", Log::L_FATAL);
	globals.SetInteger("LOG_COUNT_COUNT", Log::L_COUNT_COUNT);
	return 0;
}

void WrapLogManager::Log(LuaPlus::LuaObject lCategory, LuaPlus::LuaObject lMsg, LuaPlus::LuaObject lLevel)
{
	if(!lCategory.IsString())
	{
		CL_String category_type = lCategory.TypeName();
		CL_String err = cl_format("Failed to log message, because the type of category was %1 when expecting String", category_type);
		coreMgr->getLogMgr()->log("WrapLogManager:Log", err, Log::L_ERROR);
		return;
	}

	if(!lMsg.IsString())
	{
		CL_String msg_type = lMsg.TypeName();
		CL_String err = cl_format("Failed to log message, because the type of msg was %1 when expecting String", msg_type);
		coreMgr->getLogMgr()->log("WrapLogManager:Log", err, Log::L_ERROR);
		return;
	}

	if(!lLevel.IsString() && !lLevel.IsInteger())
	{
		CL_String level_type = lLevel.TypeName();
		CL_String err = cl_format("Failed to log message, because the type of log level was %1 when expecting String or Integer", level_type);
		coreMgr->getLogMgr()->log("WrapLogManager:Log", err, Log::L_ERROR);
		return;
	}

	CL_String category = lCategory.ToString();
	CL_String msg = lMsg.ToString();

	if(lLevel.IsString())
	{
		CL_String level = lLevel.ToString();
		coreMgr->getLogMgr()->log(category, msg, level);
	}
	else
	{
		Log::LogLevel level = (Log::LogLevel)lLevel.ToInteger();
		coreMgr->getLogMgr()->log(category, msg, level);
	}
}

#pragma once

#include <ClanLib/core.h>
#include <Lua/LuaLink.h>
#include <Lua/LuaPlus.h>
#include <Lua/LuaObject.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{
class WrapLogManager;
class WrapObjectManager;
class WrapSceneManager;
class WrapPlayerManager;
class WrapGameStateManager;
class WrapCoreManager;
class WrapComponentManager;

class ScriptManager
{
public:
	ScriptManager(Core::CoreManager *coreMgr);
	virtual ~ScriptManager();

	virtual int init();
	virtual LuaPlus::LuaStateOwner *GetGlobalState() { return &globalState; }

	virtual int doFile(const CL_String &fileName);
	virtual int doString(const CL_String &script, bool throwException = false);
	virtual CL_String getErrorOnStack();

	virtual WrapLogManager *getWLogMgr() { return wrapLogMgr; }
	virtual WrapObjectManager *getWObjMgr() { return wrapObjMgr; }
	virtual WrapSceneManager *getWSceneMgr() { return wrapSceneMgr; }
	virtual WrapPlayerManager *getWPlayerMgr() { return wrapPlayerMgr; }
	virtual WrapGameStateManager *getWGameStateMgr() { return wrapGameStateMgr; }
	virtual WrapCoreManager *getWCoreMgr() { return wrapCoreMgr; }
	virtual WrapComponentManager *getWCompMgr() { return wrapCompMgr; }

private:
	void Print(LuaPlus::LuaObject text);

	Core::CoreManager *coreMgr;

	LuaPlus::LuaStateOwner globalState;
	LuaPlus::LuaObject metaTable;

	WrapLogManager *wrapLogMgr;
	WrapObjectManager *wrapObjMgr;
	WrapSceneManager *wrapSceneMgr;
	WrapPlayerManager *wrapPlayerMgr;
	WrapGameStateManager *wrapGameStateMgr;
	WrapCoreManager *wrapCoreMgr;
	WrapComponentManager *wrapCompMgr;
};

}
}

#pragma once

#include <ClanLib/core.h>

namespace LuaPlus { class LuaStateOwner; }

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

class IManager
{
public:
	IManager(Core::CoreManager *coreMgr) {}
	virtual ~IManager() {}

	virtual int init() = 0;
	virtual LuaPlus::LuaStateOwner *GetGlobalState() = 0;

	virtual int doFile(const CL_String &fileName) = 0;
	virtual int doString(const CL_String &script, bool throwException = false) = 0;
	virtual CL_String getErrorOnStack() = 0;

	virtual WrapLogManager *getWLogMgr() = 0;
	virtual WrapObjectManager *getWObjMgr() = 0;
	virtual WrapSceneManager *getWSceneMgr() = 0;
	virtual WrapPlayerManager *getWPlayerMgr() = 0;
	virtual WrapGameStateManager *getWGameStateMgr() = 0;
	virtual WrapCoreManager *getWCoreMgr() = 0;
	virtual WrapComponentManager *getWCompMgr() = 0;
};

}
}

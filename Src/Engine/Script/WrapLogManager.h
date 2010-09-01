#pragma once

#include <Lua/LuaPlus.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{

class WrapLogManager
{
public:
	WrapLogManager(Core::CoreManager *coreMgr);
	~WrapLogManager();

	int init();

private:
	void Log(LuaPlus::LuaObject lCategory, LuaPlus::LuaObject lMsg, LuaPlus::LuaObject lLevel);

	Core::CoreManager *coreMgr;
};

}
}

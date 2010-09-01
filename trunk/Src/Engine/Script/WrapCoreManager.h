#pragma once

#include <Lua/LuaPlus.h>
#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Script
{

class WrapCoreManager
{
public:
	WrapCoreManager(Core::CoreManager *coreMgr);
	~WrapCoreManager();

	int init();

private:
	void ExitGame();

	Core::CoreManager *coreMgr;
};

}
}

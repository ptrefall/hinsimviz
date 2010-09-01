#include "WrapCoreManager.h"
#include <Engine/Core/CoreManager.h>
#include "ScriptManager.h"

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapCoreManager::WrapCoreManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

WrapCoreManager::~WrapCoreManager()
{
}

int WrapCoreManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("ExitGame", *this, &WrapCoreManager::ExitGame);

	return 0;
}

void WrapCoreManager::ExitGame()
{
	coreMgr->exit();
}

#include "WrapIGameState.h"
#include "WrapGameStateManager.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/GameState/GameStateManager.h>
#include <Engine/GameState/IGameState.h>
#include <Engine/Events/Event.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapIGameState::WrapIGameState(Core::CoreManager *coreMgr, WrapGameStateManager *wGameStateMgr, GameState::IGameState *gameState)
{
	this->coreMgr = coreMgr;
	this->wGameStateMgr = wGameStateMgr;
	this->gameState = gameState;
}

WrapIGameState::~WrapIGameState()
{
}

int WrapIGameState::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	unsigned int gameStateId = gameState->getId();
	
	LuaObject &lGameStates = wGameStateMgr->getLGameStates();
	lGameState = lGameStates.CreateTable((int)gameStateId);
	lGameState.SetInteger("id", (int)gameStateId);
	lGameState.SetString("name", gameState->getName().c_str());
	lMeta = lGameState.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);
	
	lGameState.SetLightUserData("__object", this);
	lGameState.SetMetaTable(lMeta);

	return 0;
}

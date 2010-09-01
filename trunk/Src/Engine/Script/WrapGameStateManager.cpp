#include "WrapGameStateManager.h"
#include "WrapIGameState.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include "ScriptManager.h"
#include <Engine/GameState/GameStateManager.h>
#include <Engine/GameState/IGameState.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapGameStateManager::WrapGameStateManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

WrapGameStateManager::~WrapGameStateManager()
{
}

int WrapGameStateManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("ChangeGameState", *this, &WrapGameStateManager::ChangeGameState);
	lGameStates = globals.CreateTable("GameStates");

	return 0;
}

LuaObject WrapGameStateManager::getLGameState(unsigned int id)
{
	return lGameStates.GetByIndex((int)id);
}

WrapIGameState *WrapGameStateManager::getWGameState(unsigned int id)
{
	for(unsigned int i = 0; i < wGameStates.size(); i++)
	{
		if(wGameStates[i]->getGameState()->getId() == id)
		{
			return wGameStates[i];
		}
	}
	return NULL;
}

void WrapGameStateManager::ChangeGameState(LuaPlus::LuaObject lGameState)
{
	if(!lGameState.IsTable() && !lGameState.IsString())
	{
		CL_String gamestate_type = lGameState.TypeName();
		
		CL_String err = cl_format("Failed to change game state, because the type of gamestate was %1 when expecting Table or String!", gamestate_type);
		coreMgr->getLogMgr()->log("WrapGameStateManager:ChangeGameState", err, Log::L_ERROR);
		return;
	}

	if(lGameState.IsString())
	{
		Engine::GameState::IGameState *nextState = coreMgr->getGameStateMgr()->create(lGameState.ToString());
		if(nextState == NULL)
		{
			CL_String err = cl_format("Couldn't create game state %1", lGameState.ToString());
			coreMgr->getLogMgr()->log("WrapGameStateManager:ChangeGameState", err, Log::L_ERROR);
			return;
		}
		
		int fail = coreMgr->getGameStateMgr()->changeState(nextState->getId());
		if(fail)
		{
			CL_String err = cl_format("Failed to change game state to %1", nextState->getName());
			coreMgr->getLogMgr()->log("WrapGameStateManager:ChangeGameState", err, Log::L_ERROR);
			return;
		}
	}
	else if(lGameState.IsTable())
	{
		LuaObject lGameStateId = lGameState.GetByName("id");
		if(!lGameState.IsNumber() && !lGameState.IsInteger())
		{
			CL_String gamestateid_type = lGameStateId.TypeName();
			
			CL_String err = cl_format("Failed to change game state, because the type of GameStateID was %1 when expecting Number or Integer", gamestateid_type);
			coreMgr->getLogMgr()->log("WrapGameStateManager:ChangeGameState", err, Log::L_ERROR);
			return;
		}

		int fail = coreMgr->getGameStateMgr()->changeState((unsigned int)lGameStateId.ToInteger());
		if(fail)
		{
			CL_String err = cl_format("Failed to change game state to", lGameStateId.ToInteger());
			coreMgr->getLogMgr()->log("WrapGameStateManager:ChangeGameState", err, Log::L_ERROR);
			return;
		}
	}
}

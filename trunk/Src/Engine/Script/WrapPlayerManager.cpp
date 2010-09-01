#include "WrapPlayerManager.h"
#include "WrapIPlayer.h"
#include <Engine/Core/CoreManager.h>
#include "ScriptManager.h"
#include <Engine/Player/PlayerManager.h>
#include <Engine/Player/IPlayer.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapPlayerManager::WrapPlayerManager(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

WrapPlayerManager::~WrapPlayerManager()
{
}

int WrapPlayerManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	lPlayers = globals.CreateTable("Players");
	
	Player::IPlayer *human = coreMgr->getPlayerMgr()->getHuman();

	if(human)
	{
		wHuman = new WrapIPlayer(coreMgr, this, human, true);
		wHuman->init();
		wPlayers.push_back(wHuman);
	}

	for(unsigned int i = 0; i < coreMgr->getPlayerMgr()->getAICount(); i++)
	{
		if(coreMgr->getPlayerMgr()->getAI(i))
		{
			WrapIPlayer *agent = new WrapIPlayer(coreMgr, this, coreMgr->getPlayerMgr()->getAI(i));
			agent->init();
			wPlayers.push_back(agent);
		}
	}

	return 0;
}

LuaObject WrapPlayerManager::getLPlayer(unsigned int id)
{
	return lPlayers.GetByIndex((int)id);
}

WrapIPlayer *WrapPlayerManager::getWPlayer(unsigned int id)
{
	for(unsigned int i = 0; i < wPlayers.size(); i++)
	{
		if(wPlayers[i]->getPlayer()->getId() == id)
		{
			return wPlayers[i];
		}
	}
	return NULL;
}

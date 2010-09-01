#include "PlayerManager.h"
#include "Human.h"

using namespace Engine;
using namespace Player;

PlayerManager::PlayerManager(Core::CoreManager *coreMgr)
: playerIds(0)
{
	this->coreMgr = coreMgr;
}

PlayerManager::~PlayerManager()
{
	if(human)
	{
		delete human;
		human = NULL;
	}
	for(unsigned int i = 0; i < ai.size(); i++)
	{
		IPlayer *agent = ai[i];
		if(agent)
		{
			delete agent;
			agent = NULL;
		}
	}
	ai.clear();
}

int PlayerManager::init()
{
	human = new Human(genPlayerId(), "Human", coreMgr);
	return 0;
}

IPlayer *PlayerManager::getAI(unsigned int i)
{
	if(i >= ai.size())
		return NULL;

	return ai[i];
}

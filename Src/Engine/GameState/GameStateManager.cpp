#include "GameStateManager.h"
#include "IGameState.h"
#include "GameStateFactory.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Scene/SceneManager.h>

using namespace Engine;
using namespace GameState;

GameStateManager::GameStateManager(Core::CoreManager *coreMgr)
: gameStateIds(0), currentState(NULL), factory(NULL)
{
	this->coreMgr = coreMgr;
}

GameStateManager::~GameStateManager()
{
	for(unsigned int i = 0; i < gameStates.size(); i++)
	{
		IGameState *gs = gameStates[i];
		if(gs)
		{
			delete gs;
			gs = NULL;
		}
	}
	gameStates.clear();
}

int GameStateManager::init()
{
	factory = new GameStateFactory(coreMgr);
	return 0;
}

unsigned int GameStateManager::genGameStateId()
{
	gameStateIds++;
	return gameStateIds;
}

IGameState *GameStateManager::getGameState(unsigned int id) const
{
	IGameState *gameState = NULL;
	for(unsigned int i = 0; i < gameStates.size(); i++)
	{
		if(gameStates[i]->getId() == id)
		{
			gameState = gameStates[i];
			break;
		}
	}
	return gameState;
}

IGameState *GameStateManager::getCurrentState() const
{
	return currentState;
}

int GameStateManager::changeState(unsigned int id)
{
	if(currentState && (currentState->getId() == id)) //Can't change to same state
		return 1;

	IGameState *nextState = NULL;
	for(unsigned int i = 0; i < gameStates.size(); i++)
	{
		if(gameStates[i]->getId() == id)
		{
			nextState = gameStates[i];
			break;
		}
	}

	if(nextState == NULL)
		return 1;

	if(currentState)
	{
		currentState->cleanup();
	}

	currentState = nextState;
	coreMgr->getSceneMgr()->setGameStateRoot(currentState->getGuiRoot(), currentState->getRoot());
	currentState->initScript();
	currentState->resume();
	return 0;
}

int GameStateManager::pushState(unsigned int id)
{
	if(currentState && (currentState->getId() == id)) //Can't change to same state
		return 1;

	IGameState *nextState = NULL;
	for(unsigned int i = 0; i < gameStates.size(); i++)
	{
		if(gameStates[i]->getId() == id)
		{
			nextState = gameStates[i];
			break;
		}
	}

	if(nextState == NULL)
		return 1;

	if(currentState)
	{
		currentState->pause();
		gameStateStackHistory.push_back(currentState);
	}

	currentState = nextState;
	coreMgr->getSceneMgr()->setGameStateRoot(currentState->getGuiRoot(), currentState->getRoot());
	currentState->init();
	currentState->resume();
	return 0;
}

int GameStateManager::popState()
{
	if(gameStateStackHistory.empty())
		return 1;

	if(currentState)
	{
		currentState->pause();
	}

	currentState = gameStateStackHistory[gameStateStackHistory.size()-1];
	gameStateStackHistory.erase(gameStateStackHistory.end()-1);
	coreMgr->getSceneMgr()->setGameStateRoot(currentState->getGuiRoot(), currentState->getRoot());
	currentState->resume();
	return 0;
}

IGameState *GameStateManager::create(const CL_String &name)
{
	if(factory == NULL)
		return NULL;

	//Check if gamestate has already been loaded
	for(unsigned int i = 0; i < gameStates.size(); i++)
	{
		if(gameStates[i]->getName() == name)
		{
			return gameStates[i];
		}
	}

	IGameState *state = factory->create(name.c_str());
	if(state)
		gameStates.push_back(state);
	return state;
}

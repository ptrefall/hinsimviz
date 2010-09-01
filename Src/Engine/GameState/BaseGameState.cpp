#include "BaseGameState.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Node.h>
#include <Engine/Script/ScriptManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/XMLResource.h>

using namespace Engine;
using namespace GameState;

BaseGameState::BaseGameState(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr)
: IGameState(id, name, coreMgr), guiRoot(NULL), root(NULL)//, terrain(NULL)
{
	this->coreMgr = coreMgr;
	this->id = id;
	this->name = name;
}

BaseGameState::~BaseGameState()
{
	if(guiRoot)
	{
		delete guiRoot;
		guiRoot = NULL;
	}
	if(root)
	{
		delete root;
		root = NULL;
	}
}

int BaseGameState::init()
{
	guiRoot = coreMgr->getSceneMgr()->create("Root");
	int fail = guiRoot->init();
	if(fail)
	{
		CL_String msg = cl_format("An error occured initializing the gui root node of game state %1", name);
		coreMgr->getLogMgr()->log("BaseGameState::init", msg, Log::L_ERROR);
		return fail;
	}

	root = coreMgr->getSceneMgr()->create("Root");
	fail = root->init();
	if(fail)
	{
		CL_String msg = cl_format("An error occured initializing the root node of game state %1", name);
		coreMgr->getLogMgr()->log("BaseGameState::init", msg, Log::L_ERROR);
		return fail;
	}

	return 0;
}

int BaseGameState::initScript()
{
	int fail = coreMgr->getScriptMgr()->doString(cl_format("%1:init()", name));
	if(fail)
	{
		CL_String msg = cl_format("Failed to load GameState init script %1", cl_format("%1:init()", name));
		coreMgr->getLogMgr()->log("BaseGameState::initScript", msg, Log::L_ERROR);
		return fail;
	}
	return 0;
}

int BaseGameState::cleanup()
{
	return 0;
}

int BaseGameState::resume()
{
	return 0;
}

int BaseGameState::pause()
{
	return 0;
}

int BaseGameState::update(double dt)
{
	return root->update(dt);
}

int BaseGameState::display()
{
	return root->display();
}

#include "CoreManager.h"

//Ifdef this if running on multiple platforms...
#include "Win32Timer.h"

#include <ClanLib/core.h>
#include <GL/glew.h>
#include <Engine/GUI/IMainGuiManager.h>
#include <Engine/GUI/IGuiManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/EngineEventManager.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Script/ScriptManager.h>
#include <Engine/Player/PlayerManager.h>
#include <Engine/GameState/GameStateManager.h>
#include <Engine/GameState/IGameState.h>
#include <Engine/Events/EngineEvent.h>

using namespace Engine;
using namespace Core;

CoreManager::CoreManager(int argc, char *argv[], GUI::IMainGuiManager *mainGuiMgr, GUI::IGuiManager *guiMgr)
: setupCore(new CL_SetupCore()), 
  resMgr(NULL), sceneMgr(NULL), objMgr(NULL), playerMgr(NULL), gameStateMgr(NULL), scriptMgr(NULL),
  engineEventMgr(NULL), logMgr(NULL), timer(NULL)
{
	CL_String arg = argv[0];
	arg = arg.substr(0, arg.find_last_of("\\"));
	arg = arg.substr(0, arg.find_last_of("\\"));
	arg = arg.substr(0, arg.find_last_of("\\"));
	//arg = arg.substr(0, arg.find_last_of("\\"));
	arg += "\\Resources\\";

	this->mainGuiMgr = mainGuiMgr;
	this->guiMgr = guiMgr;
	
	try
	{
		init(arg.c_str());
	}
	catch(const CL_Exception &e)
	{
		if(logMgr)
		{
			logMgr->log("CoreManager::CoreManager", e.what(), Log::L_FATAL);
		}
		else
		{
			std::cout << "CL_EXCEPTION: " << e.what() << std::endl;
		}
	}
}

CoreManager::~CoreManager()
{
	if(resMgr)
	{
		delete resMgr;
		resMgr = NULL;
	}

	/*if(inputMgr)
	{
		delete inputMgr;
		inputMgr = NULL;
	}*/

	if(sceneMgr)
	{
		delete sceneMgr;
		sceneMgr = NULL;
	}

	if(objMgr)
	{
		delete objMgr;
		objMgr = NULL;
	}

	/*if(soundMgr)
	{
		delete soundMgr;
		soundMgr = NULL;
	}*/

	if(playerMgr)
	{
		delete playerMgr;
		playerMgr = NULL;
	}

	if(gameStateMgr)
	{
		delete gameStateMgr;
		gameStateMgr = NULL;
	}

	if(scriptMgr)
	{
		delete scriptMgr;
		scriptMgr = NULL;
	}

	if(engineEventMgr)
	{
		delete engineEventMgr;
		engineEventMgr = NULL;
	}

	if(logMgr)
	{
		delete logMgr;
		logMgr = NULL;
	}

	delete setupCore;
	setupCore = NULL;

	if(timer)
	{
		timer->stop();
		delete timer;
		timer = NULL;
	}
}

void CoreManager::init(const char *arg)
{
	//RESOURCE MANAGER //////////////////////////////////////////////////////////////////////
	resMgr = new Resource::ResManager();
	int fail = resMgr->init(arg);
	if(fail)
		throw CL_Exception("Initializing the Resource Manager failed!");

	//LOG MANAGER //////////////////////////////////////////////////////////////////////
	logMgr = new Engine::Log::LogManager(this);
	fail = logMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Log Manager failed!");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		const unsigned char *errString = glewGetErrorString(err);
		CL_String error = cl_format("Error loading GLEW: %1", (const char*)errString);
		logMgr->log("AppManager:Init", error, Log::L_FATAL);
		throw CL_Exception("Initializing GLEW failed!");
	}

	//INPUT MANAGER //////////////////////////////////////////////////////////////////////
	/*inputMgr = new Engine::Input::OISManager(this);
	fail = inputMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Input Manager failed!");*/

	//ENGINE EVENT MANAGER //////////////////////////////////////////////////////////////////////
	engineEventMgr = new Events::EngineEventManager();
	fail = engineEventMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Engine Event Manager failed!");

	//SOUND MANAGER //////////////////////////////////////////////////////////////////////
	/*soundMgr = new Engine::Sound::SoundManager(this);
	fail = soundMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Sound Manager failed!");*/

	//SCENE MANAGER //////////////////////////////////////////////////////////////////////
	sceneMgr = new Engine::Scene::SceneManager(this);
	fail = sceneMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Scene Manager failed!");

	//OBJECT MANAGER //////////////////////////////////////////////////////////////////////
	objMgr = new Engine::Scene::ObjectManager(this);
	fail = objMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Object Manager failed!");

	//PLAYER MANAGER //////////////////////////////////////////////////////////////////////
	playerMgr = new Engine::Player::PlayerManager(this);
	fail = playerMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Player Manager failed!");

	//SCRIPT MANAGER //////////////////////////////////////////////////////////////////////
	scriptMgr = new Engine::Script::ScriptManager(this);
	fail = scriptMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Script Manager failed!");

	//GAME STATE MANAGER //////////////////////////////////////////////////////////////////////
	gameStateMgr = new Engine::GameState::GameStateManager(this);
	fail = gameStateMgr->init();
	if(fail)
		throw CL_Exception("Initializing the Game State Manager failed!");

	//Initialize scene
	logMgr->log("GameManager::init", "Loading Game Configuration from file", Engine::Log::L_INFO);

	Resource::IResource *config = resMgr->create("game_config.xml", "XML");
	CL_String startState = config->getString("Config/Game/StartState");
	
	logMgr->log("GameManager::init", cl_format("Start state is %1", startState), Engine::Log::L_INFO);
	
	Engine::GameState::IGameState *state = gameStateMgr->create(startState);
	if(state)
	{
		gameStateMgr->changeState(state->getId());
	}

	logMgr->log("GameManager::init", "Finished loading Game Configuration from file", Engine::Log::L_INFO);

	timer = new Win32Timer();
	timer->start();
}

void CoreManager::resize(int w, int h)
{
	sceneMgr->reseize(w,h);
}

void CoreManager::frame()
{
	double dt = (float)timer->update();
	if(dt > 100.0)
		dt = 100.0; //Clamp at 100, so that we don't get insanse steps in certain situations that might occure

	sceneMgr->update(dt);
	sceneMgr->display();
	guiMgr->swapBuffers();
	mainGuiMgr->setCaptionText(cl_format("FPS: %1", timer->getFps()).c_str());
}

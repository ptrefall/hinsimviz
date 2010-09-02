#pragma once

class CL_SetupCore;

namespace Engine
{
namespace GUI { class IGuiManager; }
namespace Resource { class ResManager; }
namespace Log { class LogManager; }
namespace Events { class IEngineEventManager; }
namespace Scene { class SceneManager; class ObjectManager; }
namespace Script { class ScriptManager; }
//namespace Sound { class SoundManager; }
namespace Player { class PlayerManager; }
namespace GameState { class GameStateManager; }
namespace Core
{
	class ITimer;
	class CoreManager
	{
	public:
		CoreManager(int argc, char *argv[], GUI::IGuiManager *guiManager);
		~CoreManager();

		void resize(int w, int h);
		void frame();
		void exit() {}

		GUI::IGuiManager *getGuiMgr() const { return guiManager; }
		Resource::ResManager *getResMgr() const { return resMgr; }
		Events::IEngineEventManager *getEngineEventMgr() const { return engineEventMgr; }
		Log::LogManager *getLogMgr() const { return logMgr; }
		//Input::IManager *getInputMgr() const { return inputMgr; }
		Scene::ObjectManager *getObjectMgr() const { return objMgr; }
		Scene::SceneManager *getSceneMgr() const { return sceneMgr; }
		Script::ScriptManager *getScriptMgr() const { return scriptMgr; }
		//Sound::SoundManager *getSoundMgr() const { return soundMgr; }
		Player::PlayerManager *getPlayerMgr() const { return playerMgr; }
		GameState::GameStateManager *getGameStateMgr() const { return gameStateMgr; }

		//Utility::ITimer *getTimer() const { return timer; }

	private:
		void init(const char *arg);

		GUI::IGuiManager *guiManager;

		Resource::ResManager *resMgr;
		Events::IEngineEventManager *engineEventMgr;
		Log::LogManager *logMgr;
		//Input::IManager *inputMgr;
		Scene::ObjectManager *objMgr;
		Scene::SceneManager *sceneMgr;
		Script::ScriptManager *scriptMgr;
		//Sound::SoundManager *soundMgr;
		Player::PlayerManager *playerMgr;
		GameState::GameStateManager *gameStateMgr;

		CL_SetupCore *setupCore; // Initializes clanlib core lib when CoreManager is instanciated/constructed

		ITimer *timer;
		double timeAccumulator;
		double timeStep;

		bool isRunning;
	};
}
}

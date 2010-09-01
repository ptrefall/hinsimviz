#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class IManager; }
namespace GameState
{
class IGameState;

class IManager
{
public:
	IManager(Core::IManager *coreMgr) {}
	virtual ~IManager() {}

	virtual int init() = 0;

	virtual unsigned int genGameStateId() = 0;
	virtual IGameState *getGameState(unsigned int id) const = 0;
	virtual IGameState *getCurrentState() const = 0;

	virtual int changeState(unsigned int id) = 0;
	virtual int pushState(unsigned int id) = 0;
	virtual int popState() = 0;

	virtual IGameState *create(const CL_String &name) = 0;
};

}
}

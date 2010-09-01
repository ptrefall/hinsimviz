#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class IManager; }
namespace GameState
{
class IGameState;

class IFactory
{
public:
	IFactory(Core::IManager *coreMgr) {}
	virtual ~IFactory() {}

	virtual IGameState *create(const char *name) = 0;
};

}
}

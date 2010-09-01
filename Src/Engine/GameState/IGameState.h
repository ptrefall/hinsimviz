#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Node; }
//namespace Object { class TerrainObject; }
namespace GameState
{

class IGameState
{
public:
	IGameState(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr) {}
	virtual ~IGameState() {}

	virtual unsigned int getId() const = 0;
	virtual CL_String getName() const = 0;
	virtual Scene::Node *getGuiRoot() const = 0;
	virtual Scene::Node *getRoot() const = 0;
	//virtual Object::TerrainObject *getTerrain() const = 0;
	//virtual void setTerrain(Object::TerrainObject *terrain) = 0;

	virtual int init() = 0;
	virtual int initScript() = 0;
	virtual int cleanup() = 0; //Unload all content contained by this game state

	virtual int resume() = 0; //Enter this game state, as in, it should start rendering and receiving updates
	virtual int pause() = 0; //Leave this game state, as in, it should stop rendering and receiving updates (paused)

	virtual int update(double dt) = 0;
	virtual int display() = 0;
};

}
}

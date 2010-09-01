#pragma once

#include "IGameState.h"

namespace Engine
{
namespace GameState
{

class BaseGameState : public IGameState
{
public:
	BaseGameState(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr);
	virtual ~BaseGameState();

	virtual unsigned int getId() const { return id; }
	virtual CL_String getName() const { return name; }
	virtual Scene::Node *getGuiRoot() const { return guiRoot; }
	virtual Scene::Node *getRoot() const { return root; }
	//virtual Object::TerrainObject *getTerrain() const { return terrain; }
	//virtual void setTerrain(Object::TerrainObject *terrain) { this->terrain = terrain; }

	virtual int init();
	virtual int initScript();
	virtual int cleanup(); //Unload all content contained by this game state

	virtual int resume(); //Enter this game state, as in, it should start rendering and receiving updates
	virtual int pause(); //Leave this game state, as in, it should stop rendering and receiving updates (paused)

	virtual int update(double dt);
	virtual int display();

private:
	Core::CoreManager *coreMgr;

	unsigned int id;
	CL_String name;
	Scene::Node *guiRoot;
	Scene::Node *root;
	//Object::TerrainObject *terrain;
};

}
}

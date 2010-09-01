#pragma once

#include "IPlayer.h"
#include <vector>

namespace Engine
{
namespace Player
{

class Agent : public IPlayer
{
public:
	Agent(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr);
	virtual ~Agent();

	virtual unsigned int getId() { return id; }
	virtual const CL_String &getName() { return name; }

	virtual void addGO(Scene::Object *go);
	virtual void removeGO(Scene::Object *go);
	virtual Scene::Object *getGO(unsigned int i);
	virtual unsigned int getGOCount() const;

	virtual void sendCommand(const CL_String &command);
	virtual void sendEvent(const Events::IEvent &event);

	virtual void hover(const CL_Vec3i &abs, const CL_Vec3i &rel) {}

	virtual CL_Vec3f getFocusPoint() { return CL_Vec3f(); }
	virtual CL_Vec3f getRelativeFocus() { return CL_Vec3f(); }

private:
	Core::CoreManager *coreMgr;
	unsigned int id;
	CL_String name;

	std::vector<Scene::Object*> gos;
};

}
}

#pragma once

#include <Engine/Events/IEvent.h>
#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Object; }
namespace Player
{

class IPlayer
{
public:
	IPlayer(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr) {}
	virtual ~IPlayer() {}

	virtual unsigned int getId() = 0;
	virtual const CL_String &getName() = 0;

	virtual void addGO(Scene::Object *go) = 0;
	virtual void removeGO(Scene::Object *go) = 0;
	virtual Scene::Object *getGO(unsigned int i) = 0;
	virtual unsigned int getGOCount() const = 0;

	virtual void sendCommand(const CL_String &command) = 0;
	virtual void sendEvent(const Events::IEvent &event) = 0;

	virtual void hover(const CL_Vec3i &abs, const CL_Vec3i &rel) = 0;

	virtual CL_Vec3f getFocusPoint() = 0;
	virtual CL_Vec3f getRelativeFocus() = 0;
};

}
}

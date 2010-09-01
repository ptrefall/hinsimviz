#pragma once

#include "Node.h"
#include "ObjectFactory.h"

#include <Depends/Entity/ComponentContainer.h>
#include <Depends/Entity/ComponentFactory.h>
#include <Engine/Events/IEvent.h>

namespace Engine
{
namespace Scene
{

class Object : public Entity::ComponentContainer, public Node
{
protected:
	friend class ObjectFactory;
public:
	virtual ~Object();

	virtual CL_String getNodeType() { return "Object"; }

	virtual int kill();
	virtual int update(double dt);
	virtual int interpolate(double dt);

	virtual void executeCommand(const CL_String &command, Engine::Player::IPlayer *player);
	virtual void executeEvent(const Engine::Events::IEvent &event, Engine::Player::IPlayer *player);

protected:
	Object(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Core::CoreManager *coreMgr, Entity::ComponentFactory *factory);
};

}
}

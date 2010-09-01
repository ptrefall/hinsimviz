#include "Object.h"
#include <Engine/Core/CoreManager.h>
#include "SceneManager.h"
#include "ObjectManager.h"

using namespace Engine;
using namespace Scene;

Object::Object(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Core::CoreManager *coreMgr, Entity::ComponentFactory *factory)
: Entity::ComponentContainer(*factory), Node(id, colorId, type, name, coreMgr)
{
	this->setAsObject();
}

Object::~Object()
{
}

int Object::kill()
{
	coreMgr->getSceneMgr()->registerForDelete(this);
	return 0;
}

int Object::update(double dt)
{
	int fail = Node::update(dt);
	if(fail)
		return fail;

	this->UpdateComponents(dt);
	return 0;
}

int Object::interpolate(double dt)
{
	int fail = Node::interpolate(dt);
	if(fail)
		return fail;

	this->InterpolateComponents(dt);
	return 0;
}

void Object::executeCommand(const CL_String &command, Engine::Player::IPlayer *player)
{
	this->ExecuteCommandOnComponents(command, player);
}

void Object::executeEvent(const Engine::Events::IEvent &event, Engine::Player::IPlayer *player)
{
	this->ExecuteEventOnComponents(event, player);
}

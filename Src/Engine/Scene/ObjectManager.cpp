#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "ObjectRegistrator.h"
#include "ComponentRegistrator.h"
#include "Object.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>
#include <Depends/Entity/ComponentFactory.h>
#include <Engine/Player/PlayerManager.h>
#include <Engine/Player/IPlayer.h>

using namespace Engine;
using namespace Scene;

ObjectManager::ObjectManager(Core::CoreManager *coreMgr)
: factory(NULL), compFactory(NULL)
{
	this->coreMgr = coreMgr;
}

ObjectManager::~ObjectManager()
{
	if(factory)
	{
		delete factory;
		factory = NULL;
	}
	if(compFactory)
	{
		delete compFactory;
		compFactory = NULL;
	}
}

int ObjectManager::init()
{
	compFactory = new Entity::ComponentFactory(coreMgr);
	ComponentRegistrator::Register(compFactory);
	factory = new ObjectFactory(coreMgr);
	ObjectRegistrator::Register(factory, coreMgr);

	return 0;
}

Object *ObjectManager::create(const CL_String &type, const CL_String &name)
{
	if(factory == NULL)
		return NULL;
	coreMgr->getLogMgr()->log("ObjectManager::Create", cl_format("Adding %1 object of type %2 to Scene", name, type), Log::L_INFO);

	Object *obj = factory->create(type, name);
	coreMgr->getEngineEventMgr()->SendEvent(Engine::Events::EngineEvent("ObjectCreated", Engine::Events::EngineEventValue(obj)));
	factory->addDataAndLogic(obj, type);

	coreMgr->getLogMgr()->log("ObjectManager::Create", cl_format("Finished adding %1 object of type %2 to Scene", name, type), Log::L_INFO);
	return obj;
}

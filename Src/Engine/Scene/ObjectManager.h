#pragma once

#include <vector>
#include <Engine/Events/EngineEventContainer.h>
#include <Engine/Events/EngineEvent.h>
#include <Depends/Entity/ComponentFactory.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
class ObjectFactory;

class ObjectManager
{
public:
	ObjectManager(Core::CoreManager *coreMgr);
	virtual ~ObjectManager();

	virtual int init();
	virtual Object *create(const CL_String &type, const CL_String &name = CL_String());
	virtual Entity::ComponentFactory *getComponentFactory() const { return compFactory; }

private:
	Core::CoreManager *coreMgr;
	ObjectFactory *factory;
	Entity::ComponentFactory *compFactory;
};

}
}

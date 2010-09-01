#include "Root.h"

using namespace Engine;
using namespace Scene;

Root::Root(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr)
: Engine::Scene::Node(id, colorId, type, name, coreMgr)
{
	renderable = false;
	opaque = true;
	drawMode = 0;
}

Root::~Root()
{
}

int Root::init()
{
	initialized = true;
	return 0;
}

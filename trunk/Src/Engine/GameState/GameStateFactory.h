#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Node; }
namespace GameState
{
class IGameState;
class GameStateFactory
{
public:
	GameStateFactory(Core::CoreManager *coreMgr);
	virtual ~GameStateFactory();

	virtual IGameState *create(const char *name);

private:
	void loadObjects(Scene::Node *root, const CL_DomNode &domNode, const CL_String &name);
	void loadNodes(Scene::Node *root, const CL_DomNode &domNode, const CL_String &name);
	void loadProperties(Scene::Node *node, const CL_DomNode &xmlNode, const CL_String &name);

	Core::CoreManager *coreMgr;
};

}
}

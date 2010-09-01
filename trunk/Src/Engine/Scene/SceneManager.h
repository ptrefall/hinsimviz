#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
class Node;
class NodeFactory;
class Camera;
class PostProcess;

class SceneManager
{
public:
	SceneManager(Core::CoreManager *coreMgr);
	virtual ~SceneManager();

	virtual int init();
	virtual Node *create(const CL_String &type, const CL_String &name = CL_String());
	virtual void registerForDelete(Node *node);
	virtual void checkForPendingDelete();
	virtual unsigned int genNodeId();
	virtual CL_Vec4f genNodeColorId();
	virtual NodeFactory *getNodeFactory() const { return factory; }
	
	virtual Camera *getCamera() const { return camera; }
	virtual void setActiveCamera(Camera *camera) { this->camera = camera; }

	virtual int update(double dt);
	virtual int interpolate(double dt);
	virtual int display();

	virtual int setGameStateRoot(Node *guiRoot, Node *root) { this->gameStateGuiRoot = guiRoot; this->gameStateRoot = root; return 0; }
	virtual Node *getGuiRoot() const { return gameStateGuiRoot; }
	virtual Node *getRoot() const { return gameStateRoot; }

	virtual PostProcess *getScenePostProcess() const { return scenePostProcess; }

private:
	Core::CoreManager *coreMgr;
	NodeFactory *factory;
	unsigned int nodeIds;
	CL_Vec4f colorIds;

	Node *gameStateGuiRoot;
	Node *gameStateRoot;

	Node *root; //Placeholder
	Camera *camera; //Placeholder

	PostProcess *scenePostProcess;

	std::vector<Node*> pendingDelete;
};

}
}

#include "GameStateFactory.h"
#include "GameStateManager.h"
#include "BaseGameState.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/XMLResource.h>
#include <Engine/Script/ScriptManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Node.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>
#include <Depends/Entity/IProperty.h>

using namespace Engine;
using namespace GameState;

GameStateFactory::GameStateFactory(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

GameStateFactory::~GameStateFactory()
{
}

IGameState *GameStateFactory::create(const char *name)
{
	//Load XML resource for this game state
	Resource::IResource *res = coreMgr->getResMgr()->create(cl_format("%1/%2.xml", "GameStates", name).c_str(), "XML");
	CL_String n = res->getString("GameState/Name");
	if(n != CL_String(name))
	{
		return NULL;
	}

	IGameState *gameState = new BaseGameState(coreMgr->getGameStateMgr()->genGameStateId(), name, coreMgr);
	gameState->init();

	//Load all scripts for this game state
	std::vector<CL_String> scripts = coreMgr->getResMgr()->getFilesInDir(cl_format("/Script/GameStates/%1/", name).c_str());
	for(unsigned int i = 0; i < scripts.size(); i++)
	{
		int fail = coreMgr->getScriptMgr()->doFile(cl_format("GameStates/%1/%2", name, scripts[i]));
		if(fail)
		{
			CL_String msg = cl_format("Failed to load script %1", scripts[i]);
			coreMgr->getLogMgr()->log("GameStateFactory::create", msg, Log::L_ERROR);
			continue;
		}
	}

	Resource::XMLResource *cl_res = static_cast<Resource::XMLResource*>(res);
	CL_DomNode guiNode = cl_res->getDoc().select_node("GameState/Gui");
	CL_DomNode sceneNode = cl_res->getDoc().select_node("GameState/Scene");

	loadObjects(gameState->getGuiRoot(), guiNode, name);
	loadNodes(gameState->getGuiRoot(), guiNode, name);

	loadObjects(gameState->getRoot(), sceneNode, name);
	loadNodes(gameState->getRoot(), sceneNode, name);

	return gameState;
}

void GameStateFactory::loadObjects(Scene::Node *root, const CL_DomNode &domNode, const CL_String &name)
{
	std::vector<CL_DomNode> objects = domNode.select_nodes("Objects/Object");
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		CL_String objType = objects[i].select_string("Type");
		CL_String objName = CL_String();
		try
		{
			objName = objects[i].select_string("Name");
		}
		catch(const CL_Exception &)
		{
		}

		//Create an instance of the node
		Scene::Object *obj = coreMgr->getObjectMgr()->create(objType, objName);
		if(obj == NULL)
		{
			CL_String msg = cl_format("Failed to create object %1 of type %2 for game state %3", objName, objType, name);
			coreMgr->getLogMgr()->log("GameStateFactory::loadObjects", msg, Log::L_ERROR);
			continue;
		}

		obj->init();
		loadProperties(static_cast<Scene::Node*>(obj), objects[i], name);
		root->add(obj);
	}
}

void GameStateFactory::loadNodes(Scene::Node *root, const CL_DomNode &domNode, const CL_String &name)
{
	std::vector<CL_DomNode> nodes = domNode.select_nodes("Nodes/Node");
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		CL_DomElement nodeType = nodes[i].to_element();

		CL_String nodeName = nodeType.get_text();

		//Create an instance of the node
		Scene::Node *node = coreMgr->getSceneMgr()->create(nodeName);
		if(node == NULL)
		{
			CL_String msg = cl_format("Failed to create node %1 for game state %3", nodeName, name);
			coreMgr->getLogMgr()->log("GameStateFactory::loadNodes", msg, Log::L_ERROR);
			continue;
		}

		node->init();
		loadProperties(node, nodes[i], name);
		root->add(node);
	}
}

void GameStateFactory::loadProperties(Scene::Node *node, const CL_DomNode &xmlNode, const CL_String &name)
{
	std::vector<CL_DomNode> properties = xmlNode.select_nodes("Properties/Property");
	for(unsigned int i = 0; i < properties.size(); i++)
	{
		CL_DomElement propType = properties[i].to_element();
		CL_String propName = propType.get_child_string("Name");
		CL_String propValue = propType.get_child_string("Value");

		//Can't set the value of a property that doesn't exist!
		if(!node->HasProperty(propName))
			continue;

		Entity::IProperty *propInterface = node->GetIProperty(propName);
		propInterface->SetFromString(propValue);
	}
}

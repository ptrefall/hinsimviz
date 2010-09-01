#include "NodeFactory.h"
#include "SceneManager.h"
#include "Node.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>

#include <ClanLib/core.h>

using namespace Engine;
using namespace Scene;

std::map<CL_String, NodeFactory::NodeCreator>* NodeFactory::creators;

NodeFactory::NodeFactory(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

NodeFactory::~NodeFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
	}
}

void NodeFactory::registerNode(const CL_String &type, NodeCreator functor)
{
	if(creators == 0)
		creators = new std::map<CL_String, NodeCreator>();

	if(creators->find(type) == creators->end())
	{
		std::pair<CL_String, NodeCreator> value(type, functor);
		creators->insert(value);
	}
}

Node *NodeFactory::create(const CL_String &type, const CL_String &name)
{
	if(creators == 0)
		throw CL_Exception("NodeCreator map has not been instanciated!");

	NodeCreator creator = NULL;
	std::map<CL_String, NodeCreator>::iterator creatorIt = creators->begin();
	for(; creatorIt != creators->end(); ++creatorIt)
	{
		if(creatorIt->first.compare(type) == 0)
		{
			creator = creatorIt->second;
		}
	}

	if(creator == NULL)
	{
		CL_String msg = cl_format("Error: Unable to create node %1 of type %2, no such creator in creators", name, type);
		coreMgr->getLogMgr()->log("NodeFactory::Create", msg, Log::L_ERROR);
		return NULL;
	}
	
	Node *node = creator(coreMgr->getSceneMgr()->genNodeId(), coreMgr->getSceneMgr()->genNodeColorId(), type, name, coreMgr);
	return node;
}

#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Object.h"
#include "SceneManager.h"
#include "Node.h"

#include <Engine/Core/CoreManager.h>

#include <Engine/Resource/ResManager.h>
#include <Engine/Resource/IResource.h>
#include <Engine/Resource/XMLResource.h>

#include <Depends/Entity/TypeSerializer.h>
#include <Engine/Log/LogManager.h>

#include <ClanLib/core.h>

using namespace Engine;
using namespace Scene;

std::map<CL_String, CL_String>* ObjectFactory::creators;

ObjectFactory::ObjectFactory(Core::CoreManager *coreMgr)
{
	this->coreMgr = coreMgr;
}

ObjectFactory::~ObjectFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
		creators = NULL;
	}
}

void ObjectFactory::registerObject(const char *fileName)
{
	if(creators == 0)
		creators = new std::map<CL_String, CL_String>();

	//Check if file has already been loaded, though fileName is registered as
	//second value in the map, because accessing the first value is faster, and
	//we need that speed in the run-time create() function more than we need that
	//speed at register initialization.
	std::map<CL_String, CL_String>::iterator it = creators->begin();
	for(; it != creators->end(); ++it)
	{
		if((*it).second == fileName)
		{
			return;
		}
	}

	CL_String type = loadObject(fileName).c_str();

	std::pair<CL_String, CL_String> value(type, fileName);
	creators->insert(value);
}

Object *ObjectFactory::create(const CL_String &type, const CL_String &name)
{
	if(creators == 0)
		throw CL_Exception("ObjectCreator map has not been instanciated!");

	std::map<CL_String, CL_String>::iterator creatorIt = creators->find(type);
	if(creatorIt == creators->end())
		throw CL_Exception(cl_format("%1 %2", "Unable to create object of type", type));
	
	Object *go = new Object(coreMgr->getSceneMgr()->genNodeId(), coreMgr->getSceneMgr()->genNodeColorId(), type, name, coreMgr, coreMgr->getObjectMgr()->getComponentFactory());

	CL_String shader = object_shader.find(type)->second;
	if(shader != CL_String())
	{
		go->setShaderName(shader);
	}

	return go;
}

void ObjectFactory::addDataAndLogic(Object *go, const CL_String &type)
{
	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Adding Nodes", Log::L_INFO);
	int fail = addNodes(go, type);
	if(fail)
	{
		coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Failed adding Nodes", Log::L_ERROR);
		return;
	}
	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Finished adding Nodes", Log::L_INFO);

	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Adding Components", Log::L_INFO);
	fail = addComponents(go, type);
	if(fail)
	{
		coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Failed adding Components", Log::L_ERROR);
		return;
	}
	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Finished adding Components", Log::L_INFO);

	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Setting Properties", Log::L_INFO);
	fail = addProperties(go, type);
	if(fail)
	{
		coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Failed setting Properties", Log::L_ERROR);
		return;
	}
	coreMgr->getLogMgr()->log("ObjectFactory::AddDataAndLogic", "Finished setting Properties", Log::L_INFO);
}

int ObjectFactory::addNodes(Object *go, const CL_String &type)
{
	std::map<CL_String, std::vector<CL_String>>::iterator nodeIt = object_nodes.find(type);
	if(nodeIt == object_nodes.end())
	{
		return 1;
	}

	std::vector<CL_String> nodeTypes = nodeIt->second;
	for(unsigned int i = 0; i < nodeTypes.size(); i++)
	{
		try
		{
			Scene::Node *node = coreMgr->getSceneMgr()->create(nodeTypes[i]);
			if(node == NULL)
			{
				continue;
			}

			addNodeProperties(type, node, nodeTypes[i]);

			CL_String shader = object_shader.find(CL_String(type))->second;
			if(shader != CL_String())
				node->setShader(cl_format("%1/Shader/%2", coreMgr->getResMgr()->getRootPath(), shader));

			go->add(node);
			node->init();
			coreMgr->getLogMgr()->log("ObjectFactory::AddNodes", cl_format("Added node of type %1", nodeTypes[i]), Log::L_INFO);
		}
		catch(const CL_Exception &e)
		{
			CL_String err = cl_format("Failed to add node to Object of type %1: %2", type, e.what());
			coreMgr->getLogMgr()->log("ObjectFactory::AddNodes", err, Log::L_ERROR);
		}
	}

	return 0;
}

int ObjectFactory::addComponents(Object *go, const CL_String &type)
{
	std::map<CL_String, std::vector<CL_String>>::iterator compIt = object_components.find(type);
	if(compIt == object_components.end())
	{
		return 1;
	}

	std::vector<CL_String> compTypes = compIt->second;
	for(unsigned int i = 0; i < compTypes.size(); i++)
	{
		try
		{
			go->AddComponent(compTypes[i]);
			coreMgr->getLogMgr()->log("ObjectFactory::AddComponents", cl_format("Added component of type %1", compTypes[i]), Log::L_INFO);
		}
		catch(const CL_Exception &e)
		{
			CL_String err = cl_format("Failed to add component to Object of type %1: %2", type, e.what());
			coreMgr->getLogMgr()->log("ObjectFactory::AddNodes", err, Log::L_ERROR);
		}
	}

	return 0;
}

int ObjectFactory::addProperties(Object *go, const CL_String &type)
{
	std::map<CL_String, std::vector<std::pair<CL_String, CL_String>>>::iterator propIt = object_properties.find(type);
	if(propIt == object_properties.end())
	{
		return 1;
	}

	std::vector<std::pair<CL_String, CL_String>> propTypes = propIt->second;
	for(unsigned int i = 0; i < propTypes.size(); i++)
	{
		CL_String name = propTypes[i].first;
		if(!go->HasProperty(name))
			continue;

		CL_String value = propTypes[i].second;
		Entity::IProperty *propInterface = go->GetIProperty(name);
		propInterface->SetFromString(value);
		CL_String msg = cl_format("%1: %2", name, value);
		coreMgr->getLogMgr()->log("ObjectFactory::AddProperties", msg, Log::L_DEBUG);
	}

	return 0;
}

int ObjectFactory::addNodeProperties(const CL_String &objType, Scene::Node *node, const CL_String &nodeType)
{
	std::map<std::pair<CL_String, CL_String>, std::vector<std::pair<CL_String, CL_String>>>::iterator propIt = node_properties.find(std::pair<CL_String, CL_String>(objType, nodeType));
	if(propIt == node_properties.end())
	{
		return 1;
	}

	std::vector<std::pair<CL_String, CL_String>> propTypes = propIt->second;
	for(unsigned int i = 0; i < propTypes.size(); i++)
	{
		CL_String name = propTypes[i].first;
		if(!node->HasProperty(name))
			continue;

		CL_String value = propTypes[i].second;
		Entity::IProperty *propInterface = node->GetIProperty(name);
		propInterface->SetFromString(value);
		CL_String msg = cl_format("%1: %2", name, value);
		coreMgr->getLogMgr()->log("ObjectFactory::AddNodeProperties", msg, Log::L_DEBUG);
	}

	return 0;
}

CL_String ObjectFactory::loadObject(const char *fileName)
{
	Resource::IResource *res = coreMgr->getResMgr()->create(cl_format("%1/%2", "Objects", fileName).c_str(), "XML");

	CL_String type = res->getString("Object/Type");
	CL_String inherit;
	try
	{
		inherit = res->getString("Object/Inherits");
	}
	catch(const CL_Exception &)
	{
		inherit = CL_String();
	}

	//If this object inherits from another, make sure that the
	//parent object is already loaded, if not, load it before
	//we continue
	bool inheritSupported = false;
	if(inherit != CL_String())
	{
		std::map<CL_String, CL_String>::iterator creatorIt = creators->find(inherit);
		if(creatorIt == creators->end())
		{
			registerObject(cl_format("%1%2", inherit, ".xml").c_str());
			creatorIt = creators->find(inherit);
			if(creatorIt != creators->end())
			{
				inheritSupported = true;
			}
		}
		else
		{
			inheritSupported = true;
		}
	}

	CL_String shader;
	try
	{
		shader = res->getString("Object/Shader");
	}
	catch(const CL_Exception &)
	{
		shader = CL_String();
	}

	object_shader[type] = shader;

	loadNodes(res, type, inherit, inheritSupported);
	loadComponents(res, type, inherit, inheritSupported);
	loadProperties(res, type, inherit, inheritSupported);

	if(inheritSupported)
	{
		CL_String info = cl_format("Object: %1 : public %2", type, inherit);
		coreMgr->getLogMgr()->log("ObjectFactory::LoadObject", info, Log::L_DEBUG);
	}
	else
	{
		CL_String info = cl_format("Object: %1", type);
		coreMgr->getLogMgr()->log("ObjectFactory::LoadObject", info, Log::L_DEBUG);
	}

	return type;
}

void ObjectFactory::loadNodes(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported)
{
	std::vector<CL_String> nodeTypes;

	//First add inherited components
	if(inheritSupported)
	{
		std::map<CL_String, std::vector<CL_String>>::iterator inheritIt = object_nodes.find(inherit);
		if(inheritIt != object_nodes.end())
		{
			std::vector<CL_String> inheritNodeTypes = inheritIt->second;
			for(unsigned int i = 0; i < inheritNodeTypes.size(); i++)
			{
				nodeTypes.push_back(inheritNodeTypes[i]);
			}
		}
	}

	//Then add unique components
	Resource::XMLResource *cl_res = static_cast<Resource::XMLResource*>(res);
	std::vector<CL_DomNode> nodes = cl_res->getDoc().select_nodes("/Object/Nodes/Node");
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		CL_String nodeTypeText;
		CL_DomElement nodeType = nodes[i].to_element();
		int alreadyExist = -1;
	
		try
		{
			nodeTypeText = nodeType.get_child_string("Type");
		}
		catch(const CL_Exception &)
		{
		}

		if(nodeTypeText == CL_String())
		{
			nodeTypeText = nodeType.get_text();
		}

		if(nodeTypeText == CL_String())
		{
			CL_String info = cl_format("Could not extract node type text from node in XML");
			coreMgr->getLogMgr()->log("ObjectFactory::loadNodes", info, Log::L_WARN);
			return;
		}
	
		nodeTypes.push_back(nodeTypeText);
		loadProperties(type, nodeType, nodeTypeText);
	}
	if(object_nodes.find(type) != object_nodes.end())
		return;

	object_nodes[type] = nodeTypes;
}

void ObjectFactory::loadComponents(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported)
{
	std::vector<CL_String> compTypes;

	//First add inherited components
	if(inheritSupported)
	{
		std::map<CL_String, std::vector<CL_String>>::iterator inheritIt = object_components.find(inherit);
		if(inheritIt != object_components.end())
		{
			std::vector<CL_String> inheritCompTypes = inheritIt->second;
			for(unsigned int i = 0; i < inheritCompTypes.size(); i++)
			{
				compTypes.push_back(inheritCompTypes[i]);
			}
		}
	}

	//Then add unique components
	Resource::XMLResource *cl_res = static_cast<Resource::XMLResource*>(res);
	std::vector<CL_DomNode> components = cl_res->getDoc().select_nodes("/Object/Components/Component");
	for(unsigned int i = 0; i < components.size(); i++)
	{
		CL_DomElement compType = components[i].to_element();

		int alreadyExist = -1;
		for(unsigned int j = 0; j < compTypes.size(); j++)
		{
			if(compTypes[j] == compType.get_text())
			{
				alreadyExist = j;
				break;
			}
		}

		if(alreadyExist == -1)
			compTypes.push_back(compType.get_text());
	}
	object_components[type] = compTypes;
}

void ObjectFactory::loadProperties(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported)
{
	std::vector<std::pair<CL_String,CL_String>> propTypes;

	//First add inherited properties
	if(inheritSupported)
	{
		std::map<CL_String, std::vector<std::pair<CL_String,CL_String>>>::iterator inheritIt = object_properties.find(inherit);
		if(inheritIt != object_properties.end())
		{
			std::vector<std::pair<CL_String,CL_String>> inheritPropTypes = inheritIt->second;
			for(unsigned int i = 0; i < inheritPropTypes.size(); i++)
			{
				propTypes.push_back(inheritPropTypes[i]);
			}
		}
	}

	//Then add unique properties
	Resource::XMLResource *cl_res = static_cast<Resource::XMLResource*>(res);
	std::vector<CL_DomNode> properties = cl_res->getDoc().select_nodes("/Object/Properties/Property");
	for(unsigned int i = 0; i < properties.size(); i++)
	{
		CL_DomElement propType = properties[i].to_element();
		CL_String propName = propType.get_child_string("Name");
		CL_String propValue = propType.get_child_string("Value");

		int alreadyExist = -1;
		for(unsigned int j = 0; j < propTypes.size(); j++)
		{
			if(propTypes[j].first == propName)
			{
				alreadyExist = j;
				break;
			}
		}

		//If property already exist, then the last property value will always win
		if(alreadyExist == -1)
			propTypes.push_back(std::pair<CL_String,CL_String>(propName, propValue));
		else
			propTypes[alreadyExist] = std::pair<CL_String,CL_String>(propName, propValue);
	}
	object_properties[type] = propTypes;
}

void ObjectFactory::loadProperties(const CL_String &objType, const CL_DomNode &xmlNode, const CL_String &type)
{
	std::vector<std::pair<CL_String,CL_String>> propTypes;

	std::vector<CL_DomNode> properties = xmlNode.select_nodes("Properties/Property");
	for(unsigned int i = 0; i < properties.size(); i++)
	{
		CL_DomElement propType = properties[i].to_element();
		CL_String propName = propType.get_child_string("Name");
		CL_String propValue = propType.get_child_string("Value");

		propTypes.push_back(std::pair<CL_String,CL_String>(propName, propValue));
	}

	node_properties[std::pair<CL_String,CL_String>(objType, type)] = propTypes;
}

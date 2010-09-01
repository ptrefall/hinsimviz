#pragma once

#include <ClanLib/core.h>
#include <map>
#include <vector>

namespace Engine
{
namespace Resource { class IResource; }
namespace Core { class CoreManager; }
namespace Scene 
{ 
class Node;
class Object;
class ObjectFactory
{
public:
	ObjectFactory(Core::CoreManager *coreMgr);
	virtual ~ObjectFactory();

	virtual void registerObject(const char *fileName);
	virtual Object *create(const CL_String &type, const CL_String &name);
	virtual void addDataAndLogic(Object *go, const CL_String &type);

	static std::map<CL_String, CL_String>* creators;

private:
	CL_String loadObject(const char *fileName);
	void loadNodes(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported);
	void loadComponents(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported);
	void loadProperties(Resource::IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported);
	void loadProperties(const CL_String &objType, const CL_DomNode &xmlNode, const CL_String &type);
	int addNodes(Object *go, const CL_String &type);
	int addComponents(Object *go, const CL_String &type);
	int addProperties(Object *go, const CL_String &type);
	int addNodeProperties(const CL_String &objType, Scene::Node *node, const CL_String &nodeType);
	
	Core::CoreManager *coreMgr;

	std::map<CL_String, CL_String> object_shader;
	std::map<CL_String, std::vector<CL_String>> object_nodes;
	std::map<CL_String, std::vector<CL_String>> object_components;
	std::map<CL_String, std::vector<std::pair<CL_String, CL_String>>> object_properties;
	std::map<std::pair<CL_String, CL_String>, std::vector<std::pair<CL_String, CL_String>>> node_properties;
};

}
}

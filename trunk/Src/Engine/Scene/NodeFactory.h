#pragma once

#include <ClanLib/core.h>
#include <map>
#include <vector>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
class Node;
class NodeFactory
{
public:
	NodeFactory(Core::CoreManager *coreMgr);
	virtual ~NodeFactory();

	typedef Node *(* NodeCreator)(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Core::CoreManager *coreMgr);

	virtual void registerNode(const CL_String &type, NodeCreator functor);
	virtual Node *create(const CL_String &type, const CL_String &name);

	static std::map<CL_String, NodeFactory::NodeCreator>* creators;

private:
	Core::CoreManager *coreMgr;
};

}
}

#pragma once

#include <ClanLib/core.h>
#include <map>

namespace Engine
{
namespace Resource
{
class ResManager;
class IResource;

class ResFactory
{
public:
	ResFactory(ResManager *manager);
	virtual ~ResFactory();

	typedef IResource *(* ResourceCreator)(ResManager *manager, const char *fileName);
	virtual void registerResource(const char *fileType, ResourceCreator functor);
	virtual IResource *create(const char *fileName, const char *fileType);

	static std::map<CL_String, ResourceCreator>* creators;

private:
	ResManager *manager;

	std::map<CL_String, IResource*> xmlResources;
	std::map<CL_String, IResource*> binResources;
};

}
}

#include "ResFactory.h"
#include "XMLResource.h"
#include "BinResource.h"

using namespace Engine;
using namespace Resource;

std::map<CL_String, ResFactory::ResourceCreator>* ResFactory::creators;

ResFactory::ResFactory(ResManager *manager)
{
	this->manager = manager;
}

ResFactory::~ResFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
		creators = NULL;
	}
}

void ResFactory::registerResource(const char *fileType, ResourceCreator functor)
{
	if(creators == 0)
		creators = new std::map<CL_String, ResourceCreator>();

	if(creators->find(fileType) == creators->end())
	{
		std::pair<CL_String, ResourceCreator> value(fileType, functor);
		creators->insert(value);
	}
}

IResource *ResFactory::create(const char *fileName, const char *fileType)
{
	if(creators == 0)
		throw CL_Exception("ResourceCreator map has not been instanciated!");

	//If we're trying to create a resource, but it's already been loaded into memory, don't do it twice!
	bool isXml = false;
	bool isBin = false;
	if(fileType == XMLResource::getType())
	{
		isXml = true;
		std::map<CL_String, IResource*>::iterator it = xmlResources.find(fileName);
		if(it != xmlResources.end())
		{
			return it->second;
		}
	}
	else if(fileType == BinResource::getType())
	{
		isBin = true;
		std::map<CL_String, IResource*>::iterator it = binResources.find(fileName);
		if(it != binResources.end())
		{
			return it->second;
		}
	}

	std::map<CL_String, ResourceCreator>::iterator creatorIt = creators->find(fileType);
	if(creatorIt == creators->end())
		throw CL_Exception(cl_format("%1 %2", "Unable to create resouce of type", fileType));
	
	ResourceCreator creator = creatorIt->second;
	
	IResource *resource = creator(manager, fileName);
	if(resource && isXml)
	{
		xmlResources[fileName] = resource;
	}
	else if(resource && isBin)
	{
		binResources[fileName] = resource;
	}
	return resource;
}

#pragma once

#include "IResource.h"
#include <ClanLib/core.h>

namespace Engine
{
namespace Resource
{
class ResManager;

class XMLResource : public IResource
{
public:
	XMLResource(ResManager *manager, const char *fileName);
	virtual ~XMLResource();

	static const char *getType() { return "XML"; }
	static IResource *create(ResManager *manager, const char *fileName) { return new XMLResource(manager, fileName); }

	virtual const char *getResourceType() const { return getType(); }
	virtual int getInt(const char *id);
	virtual bool getBool(const char *id);
	virtual CL_String getString(const char *id);

	CL_DomDocument &getDoc() { return *doc; }

private:
	ResManager *manager;
	CL_DomDocument *doc;
};

}
}

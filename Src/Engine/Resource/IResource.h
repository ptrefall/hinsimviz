#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Resource
{
class ResManager;

class IResource
{
public:
	IResource(ResManager *manager, const char *fileName) {}
	virtual ~IResource() {}

	virtual const char *getResourceType() const = 0;

	virtual int getInt(const char *id) { return -1; }
	virtual bool getBool(const char *id) { return false; }
	virtual CL_String getString(const char *id) { return CL_String(); }

	virtual unsigned int getGLId() { return 0; }
	virtual unsigned int getDpp() { return 0; }
	virtual unsigned int getWidth() { return 0; }
	virtual unsigned int getHeight() { return 0; }
	virtual BYTE *getTexBits() { return 0; }
};

}
}

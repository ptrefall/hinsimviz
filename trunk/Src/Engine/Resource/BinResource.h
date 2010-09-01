#pragma once

#include "IResource.h"
#include <ClanLib/core.h>

namespace Engine
{
namespace Resource
{
class ResManager;

class BinResource : public IResource
{
public:
	BinResource(ResManager *manager, const char *fileName);
	virtual ~BinResource();

	static const char *getType() { return "BIN"; }
	static IResource *create(ResManager *manager, const char *fileName) { return new BinResource(manager, fileName); }

	virtual const char *getResourceType() const { return getType(); }

	virtual CL_DataBuffer *getBuffer() const { return buffer; }

private:
	ResManager *manager;
	CL_DataBuffer *buffer;
};

}
}

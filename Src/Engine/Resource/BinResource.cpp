#include "BinResource.h"
#include "ResManager.h"

using namespace Engine;
using namespace Resource;

BinResource::BinResource(ResManager *manager, const char *fileName)
: IResource(manager, fileName), buffer(NULL)
{
	this->manager = static_cast<ResManager*>(manager);

	CL_IODevice file = this->manager->getDir().open_file(cl_format("%1/%2", "Bin", fileName), CL_File::open_existing, CL_File::access_read);
	buffer = new CL_DataBuffer(file.get_size());
	file.read(buffer->get_data(), buffer->get_size());

	if(buffer->is_null())
		throw CL_Exception(cl_format("%1: %2", "Failed to load Bin Resource", fileName));
}

BinResource::~BinResource()
{
	if(buffer)
	{
		delete buffer;
		buffer = NULL;
	}
}

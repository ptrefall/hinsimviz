#include "XMLResource.h"
#include "ResManager.h"

using namespace Engine;
using namespace Resource;

XMLResource::XMLResource(ResManager *manager, const char *fileName)
: IResource(manager, fileName), doc(NULL)
{
	this->manager = static_cast<ResManager*>(manager);

	CL_IODevice file = this->manager->getDir().open_file(cl_format("%1/%2", "XML", fileName), CL_File::open_existing, CL_File::access_read);
	doc = new CL_DomDocument(file, false);
	if(doc->is_null())
		throw CL_Exception(cl_format("%1: %2", "Failed to load CL XML Resource", fileName));
}

XMLResource::~XMLResource()
{
	if(doc)
	{
		delete doc;
		doc = NULL;
	}
}

int XMLResource::getInt(const char *id)
{
	if(doc == NULL)
		return 0;

	return doc->select_int(id);
}

bool XMLResource::getBool(const char *id)
{
	if(doc == NULL)
		return false;

	return doc->select_bool(id);
}

CL_String XMLResource::getString(const char *id)
{
	if(doc == NULL)
		return "";

	return doc->select_string(id);
}

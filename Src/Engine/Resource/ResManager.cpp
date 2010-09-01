#include "ResManager.h"
#include "ResFactory.h"
#include "ResRegistrator.h"
//#include <FreeImage/FreeImage.h>

using namespace Engine;
using namespace Resource;

ResManager::ResManager()
: factory(NULL)
{
	//FreeImage_Initialise();
}

ResManager::~ResManager()
{
	if(factory)
	{
		delete factory;
		factory = NULL;
	}
	//FreeImage_DeInitialise();
}

int ResManager::init(const char *base_path)
{
	vfs = CL_VirtualFileSystem(CL_String(base_path), false);
	if(vfs.is_null())
		return 1;

	vdir = vfs.get_root_directory();
	
	factory = new ResFactory(this);
	ResRegistrator::Register(factory);
	return 0;
}

IResource *ResManager::create(const char *fileName, const char *fileType)
{
	if(factory == NULL)
		return NULL;

	return factory->create(fileName, fileType);
}

CL_String ResManager::getRootPath()
{
	return vfs.get_provider()->get_path();
}

std::vector<CL_String> ResManager::getFilesInDir(const char* dir)
{
	std::vector<CL_String> files;
	CL_VirtualDirectory vdirWithFiles = vdir.open_directory(dir);
	CL_VirtualDirectoryListing vdir_list = vdirWithFiles.get_directory_listing();
	while(vdir_list.next())
	{
		if(vdir_list.is_directory())
			continue;
		if(!vdir_list.is_readable())
			continue;

		files.push_back(vdir_list.get_filename());
	}
	return files;
}

#pragma once

#include <ClanLib/core.h>
#include <vector>

namespace Engine
{
namespace Resource
{
class ResFactory;
class IResource;

class ResManager
{
public:
	ResManager();
	virtual ~ResManager();

	virtual int init(const char *base_path);
	virtual IResource *create(const char *fileName, const char *fileType);

	virtual CL_String getRootPath();
	virtual std::vector<CL_String> getFilesInDir(const char* dir);

	CL_VirtualDirectory &getDir() { return vdir; }

private:
	CL_VirtualFileSystem vfs;
	CL_VirtualDirectory vdir;
	ResFactory *factory;
};

}
}

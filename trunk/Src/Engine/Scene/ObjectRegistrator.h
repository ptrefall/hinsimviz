#pragma once

#include "ObjectFactory.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Resource/ResManager.h>
#include <ClanLib/core.h>

namespace Engine
{
namespace Scene
{

	class ObjectRegistrator
	{
	public:
		static void Register(ObjectFactory *factory, Core::CoreManager *coreMgr)
		{
			coreMgr->getLogMgr()->log("ObjectRegistrator::Register", "Loading objects from XML", Log::L_INFO);
			std::vector<CL_String> objects = coreMgr->getResMgr()->getFilesInDir("/XML/Objects/");
			for(unsigned int i = 0; i < objects.size(); i++)
			{
				factory->registerObject(objects[i].c_str());
			}
			coreMgr->getLogMgr()->log("ObjectRegistrator::Register", "Finished loading objects from XML", Log::L_INFO);
		}
	};

}
}

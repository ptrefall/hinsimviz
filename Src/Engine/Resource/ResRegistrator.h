#pragma once

#include "ResFactory.h"

#include "XMLResource.h"
#include "BinResource.h"
//#include "FI_TexResource.h"

namespace Engine
{
namespace Resource
{
	class ResRegistrator
	{
	public:
		static void Register(ResFactory *factory)
		{
			factory->registerResource(XMLResource::getType(), &XMLResource::create);
			factory->registerResource(BinResource::getType(), &BinResource::create);
			//factory->registerResource(FI_TexResource::getType(), &FI_TexResource::create);
		}
	};
}
}

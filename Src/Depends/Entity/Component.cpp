#include "precomp.h"
#include "Component.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>

using namespace Engine;
using namespace Entity;

Component::~Component()
{
	//componentContainer->getCoreMgr()->getLogMgr()->log("Component::Destruct", cl_format("Component %1 deleted", name), Log::L_INFO);
}
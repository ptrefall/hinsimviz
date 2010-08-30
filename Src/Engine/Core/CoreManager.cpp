#include "CoreManager.h"
#include <GUI/IGUIManager.h>

using namespace Core;

CoreManager::CoreManager(GUI::IGuiManager *guiManager)
{
	this->guiManager = guiManager;
}

CoreManager::~CoreManager()
{
}

void CoreManager::frame()
{
}

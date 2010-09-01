#pragma once

#include "IEngineEventManager.h"
#include <ClanLib/core.h>
#include <vector>

namespace Engine
{
namespace Events
{

class EngineEventContainer
{
public:
	EngineEventContainer(IEngineEventManager *engineEventMgr) : engineEventMgr(engineEventMgr) {}
	~EngineEventContainer() {}

	template<class Class, class Param1>
	void Connect(const CL_String &eventName, Class *self, void(Class::*func)(Param1))
	{
		slots.push_back(engineEventMgr->GetEvent(eventName).connect(self, func));
	}

	template<class Class, class Param1, class UserData>
	void Connect(const CL_String &eventName, Class *self, void(Class::*func)(Param1, UserData), UserData userData)
	{
		slots.push_back(engineEventMgr->GetEvent(eventName).connect(self, func, userData));
	}

private:
	std::vector<CL_Slot> slots;
	IEngineEventManager *engineEventMgr;
};

}
}

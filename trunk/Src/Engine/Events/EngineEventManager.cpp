#include "EngineEventManager.h"

using namespace Engine;
using namespace Events;

EngineEventManager::CallbackClass &EngineEventManager::GetEvent(const CL_String &name)
{
	return eventHandlers[name];
}

void EngineEventManager::SendEvent(const EngineEvent &event)
{
	std::map<CL_String, CallbackClass>::iterator it;
	it = eventHandlers.find(event.getName());
	if(it != eventHandlers.end())
	{
		it->second.invoke(event);
		//cl_log_event("EngineEvent", "Published event: " + event.toString());
	}
	else
	{
		//cl_log_event("EngineEvent", "Published event (no subscribers): " + event.toString());
	}
}

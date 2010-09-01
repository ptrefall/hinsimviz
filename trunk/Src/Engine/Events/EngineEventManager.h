#pragma once

#include "IEngineEventManager.h"
#include <map>

namespace Engine
{
namespace Events
{

class EngineEventManager : public IEngineEventManager
{
public:
	virtual int init() { return 0; }

	virtual CallbackClass &GetEvent(const CL_String &name);
	virtual void SendEvent(const EngineEvent &event);

private:
	std::map<CL_String, CallbackClass> eventHandlers;
};

}
}

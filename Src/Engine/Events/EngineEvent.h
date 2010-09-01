#pragma once

#include "EngineEventValue.h"
#include <ClanLib/core.h>

namespace Engine
{
namespace Events
{

class EngineEvent
{
public:
	EngineEvent(const CL_String &name);
	EngineEvent(const CL_String &name, const EngineEventValue &arg1);
	EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2);
	EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3);
	EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3, const EngineEventValue &arg4);
	EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3, const EngineEventValue &arg4, const EngineEventValue &arg5);

	const CL_String &getName() const { return name; };

	unsigned int getArgumentCount() const;

	EngineEventValue getArgument(unsigned int index) const;

	void addArgument(const EngineEventValue &value);

	CL_String toString() const;

private:
	CL_String toString(const EngineEventValue &v) const;

	CL_String name;
	std::vector<EngineEventValue> arguments;
};

}
}

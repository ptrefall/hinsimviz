#include "EngineEvent.h"

using namespace Engine;
using namespace Events;

EngineEvent::EngineEvent(const CL_String &name)
: name(name)
{
}

EngineEvent::EngineEvent(const CL_String &name, const EngineEventValue &arg1)
: name(name)
{
	addArgument(arg1);
}

EngineEvent::EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
}

EngineEvent::EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
}

EngineEvent::EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3, const EngineEventValue &arg4)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
	addArgument(arg4);
}

EngineEvent::EngineEvent(const CL_String &name, const EngineEventValue &arg1, const EngineEventValue &arg2, const EngineEventValue &arg3, const EngineEventValue &arg4, const EngineEventValue &arg5)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
	addArgument(arg4);
	addArgument(arg5);
}

unsigned int EngineEvent::getArgumentCount() const
{
	return arguments.size();
}

EngineEventValue EngineEvent::getArgument(unsigned int index) const
{
	if (index >= arguments.size())
		throw CL_Exception(cl_format("Arguments out of bounds for engine event %1", name));
	return arguments[index];
}

void EngineEvent::addArgument(const EngineEventValue &value)
{
	arguments.push_back(value);
}

CL_String EngineEvent::toString() const
{
	CL_String eventInfo = cl_format("%1(", name);

	for(unsigned int i = 0; i < arguments.size(); ++i)
	{
		if(i > 0)
			eventInfo += ",";

		eventInfo += toString(arguments[i]);
	}

	eventInfo += ")";

	return eventInfo;
}

CL_String EngineEvent::toString(const EngineEventValue &v) const
{
	switch (v.GetType())
	{
	case EngineEventValue::null:
		return "null";
	case EngineEventValue::integer:
		return CL_StringHelp::int_to_text(v.ToInteger());
	case EngineEventValue::uinteger:
		return CL_StringHelp::uint_to_text(v.ToUinteger());
	case EngineEventValue::string:
		return "\"" + v.ToString() + "\"";
	case EngineEventValue::boolean:
		return v.ToBoolean() ? "true" : "false";
	case EngineEventValue::number:
		return CL_StringHelp::float_to_text(v.ToNumber());
	default:
		return "??" + CL_StringHelp::int_to_text(v.GetType());
	}
}

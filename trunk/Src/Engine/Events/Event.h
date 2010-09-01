#pragma once

#include "IEvent.h"
#include "Type.h"

namespace Engine
{
namespace Events
{

template<class T, class Self>
class Event : public IEvent
{
public:
	Event(const char *type, const T &arg, const Self &self) : IEvent(type), type(type), arg(arg), self(self) {}
	virtual const char *getType() const { return type; }
	virtual int getArgType() const { return Type::getType(arg); }
	virtual int getSelfType() const { return Type::getType(self); }
	const T &getArg() const { return arg; }
	const Self &getSelf() const { return self; }

protected:
	const char *type;
	const T &arg;
	const Self &self;
};

}}

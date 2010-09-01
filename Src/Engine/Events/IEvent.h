#pragma once

namespace Engine
{
namespace Events
{

class IEvent
{
public:
	IEvent(const char *type) {}
	virtual const char *getType() const = 0;
	virtual int getArgType() const = 0;
	virtual int getSelfType() const = 0;
};

}}

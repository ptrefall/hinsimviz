#include "EngineEventValue.h"

using namespace Engine;
using namespace Events;

EngineEventValue::EngineEventValue()
: type(null), valueInt(0)
{
}

EngineEventValue::EngineEventValue(int value)
: type(integer), valueInt(value)
{
}

EngineEventValue::EngineEventValue(unsigned int value)
: type(uinteger), valueUint(value)
{
}

EngineEventValue::EngineEventValue(float value)
: type(number), valueFloat(value)
{
}

EngineEventValue::EngineEventValue(const CL_String &value)
: type(string), valueString(value)
{
}

EngineEventValue::EngineEventValue(const CL_StringRef &value)
: type(string), valueString(value)
{
}

EngineEventValue::EngineEventValue(const char *value)
: type(string), valueString(value)
{
}

EngineEventValue::EngineEventValue(const wchar_t *value)
: type(string), valueString(value)
{
}

EngineEventValue::EngineEventValue(Engine::Scene::Object *value)
: type(object), valueObj(value)
{
}

EngineEventValue::EngineEventValue(Engine::Scene::Node *value)
: type(node), valueNode(value)
{
}

EngineEventValue::EngineEventValue(Engine::Entity::Component *value)
: type(component), valueComp(value)
{
}

EngineEventValue::EngineEventValue(Engine::Entity::IProperty *value)
: type(property), valueProp(value)
{
}

EngineEventValue::EngineEventValue(bool value)
: type(boolean), valueBool(value)
{
}

EngineEventValue::EngineEventValue(Type type)
: type(type), valueInt(0)
{
}

EngineEventValue::Type EngineEventValue::GetType() const
{
	return type;
}

bool EngineEventValue::IsNull() const
{
	return type == null;
}

bool EngineEventValue::IsUinteger() const
{
	return type == uinteger;
}

bool EngineEventValue::IsInteger() const
{
	return type == integer;
}

bool EngineEventValue::IsNumber() const
{
	return type == number;
}

bool EngineEventValue::IsString() const
{
	return type == string;
}

bool EngineEventValue::IsBoolean() const
{
	return type == boolean;
}

bool EngineEventValue::IsObj() const
{
	return type == object;
}

bool EngineEventValue::IsNode() const
{
	return type == node;
}

bool EngineEventValue::IsComponent() const
{
	return type == component;
}

bool EngineEventValue::IsProperty() const
{
	return type == property;
}

unsigned int EngineEventValue::ToUinteger() const
{
	if (IsUinteger())
		return valueUint;
	else
		throw CL_Exception("EngineEventValue is not an unsigned integer");
}

int EngineEventValue::ToInteger() const
{
	if (IsInteger())
		return valueInt;
	else
		throw CL_Exception("EngineEventValue is not an integer");
}

float EngineEventValue::ToNumber() const
{
	if (IsNumber())
		return valueFloat;
	else
		throw CL_Exception("EngineEventValue is not a floating point number");
}

CL_String EngineEventValue::ToString() const
{
	if (IsString())
		return valueString;
	else
		throw CL_Exception("EngineEventValue is not a string");
}

bool EngineEventValue::ToBoolean() const
{
	if (IsBoolean())
		return valueBool;
	else
		throw CL_Exception("EngineEventValue is not a boolean");
}

Engine::Scene::Object *EngineEventValue::ToObject() const
{
	if(IsObj())
		return valueObj;
	else
		throw CL_Exception("EngineEventValue is not an object");
}

Engine::Scene::Node *EngineEventValue::ToNode() const
{
	if(IsNode())
		return valueNode;
	else
		throw CL_Exception("EngineEventValue is not a node");
}

Engine::Entity::Component *EngineEventValue::ToComponent() const
{
	if(IsComponent())
		return valueComp;
	else
		throw CL_Exception("EngineEventValue is not a component");
}

Engine::Entity::IProperty *EngineEventValue::ToProperty() const
{
	if(IsProperty())
		return valueProp;
	else
		throw CL_Exception("EngineEventValue is not a property");
}

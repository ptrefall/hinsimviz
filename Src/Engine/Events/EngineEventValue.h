#pragma once

#include <ClanLib/core.h>

namespace Engine
{
namespace Scene { class Node; class Object; }
namespace Entity { class Component; class IProperty; }
namespace Events
{

class EngineEventValue
{
public:
	enum Type
	{
		null,
		integer,
		uinteger,
		string,
		boolean,
		number,
		object,
		node,
		component,
		property
	};

	EngineEventValue();
	EngineEventValue(int value);
	EngineEventValue(unsigned int value);
	EngineEventValue(float value);
	EngineEventValue(const CL_String &value);
	EngineEventValue(const CL_StringRef &value);
	EngineEventValue(const char *str);
	EngineEventValue(const wchar_t *str);
	EngineEventValue(Engine::Scene::Object *obj);
	EngineEventValue(Engine::Scene::Node *node);
	EngineEventValue(Engine::Entity::Component *comp);
	EngineEventValue(Engine::Entity::IProperty *prop);
	explicit EngineEventValue(bool value);
	EngineEventValue(Type type);

	Type GetType() const;

	bool IsNull() const;
	bool IsUinteger() const;
	bool IsInteger() const;
	bool IsNumber() const;
	bool IsString() const;
	bool IsBoolean() const;
	bool IsObj() const;
	bool IsNode() const;
	bool IsComponent() const;
	bool IsProperty() const;
	bool IsComplex() const;

	unsigned int ToUinteger() const;
	int ToInteger() const;
	float ToNumber() const;
	CL_String ToString() const;
	bool ToBoolean() const;
	Engine::Scene::Object *ToObject() const;
	Engine::Scene::Node *ToNode() const;
	Engine::Entity::Component *ToComponent() const;
	Engine::Entity::IProperty *ToProperty() const;

	inline operator unsigned int() const { return ToUinteger(); }
	inline operator int() const { return ToInteger(); }
	inline operator float() const { return ToNumber(); }
	inline operator CL_String() const { return ToString(); }
	inline operator bool() const { return ToBoolean(); }

private:
	Type type;
	union
	{
		int valueInt;
		unsigned int valueUint;
		float valueFloat;
		bool valueBool;
	};
	CL_String valueString;
	Engine::Scene::Object *valueObj;
	Engine::Scene::Node *valueNode;
	Engine::Entity::Component *valueComp;
	Engine::Entity::IProperty *valueProp;
};

}
}
